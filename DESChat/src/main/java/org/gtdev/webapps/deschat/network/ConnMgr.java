package org.gtdev.webapps.deschat.network;

import lombok.Getter;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Configuration;
import org.springframework.stereotype.Service;

import javax.net.ServerSocketFactory;
import javax.net.SocketFactory;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Random;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicInteger;

@Service
public class ConnMgr {
    private config cfg;

    @Configuration
    @Getter
    public static class config {
        @Value("${remoteserver.host}")
        private String host;

        @Value("${remoteserver.port}")
        private int port;
    }

    public static class Connection {

        public enum PeerType {
            SERVER,
            CLIENT,
            REPEATER
        }
        Socket s;
        PeerType PT = PeerType.SERVER;
        public String peerUin = null;
        private final AtomicInteger seqFactory = new AtomicInteger(new Random().nextInt(100000));
        public Connection(Socket s, PeerType peertype) {
            this.s = s;
            this.PT = peertype;
        }

        public synchronized int getNextSeq() {
            int incrementAndGet;
            synchronized (this) {
                incrementAndGet = seqFactory.incrementAndGet();
                if (incrementAndGet > Integer.MAX_VALUE) {
                    seqFactory.set(new Random().nextInt(100000) + 60000);
                }
            }
            return incrementAndGet;
        }

        private Object LockWaitFlag = new Object();
        private Object LockReceive = new Object();
        private CopyOnWriteArrayList<Integer> mWaitFlagArr = new CopyOnWriteArrayList<Integer>();
        private CopyOnWriteArrayList<NetPack> mReceivePackQueue = new CopyOnWriteArrayList<NetPack>();
        private ArrayList<String> cmdHistory = new ArrayList<>();

        public NetPack SendPack(NetPack b, int wait) {
            if(wait>0)
                synchronized (LockWaitFlag) {
                    mWaitFlagArr.add(b.header.seq);
                }
            System.out.println("SendPack Start");
            try {
                OutputStream os = s.getOutputStream();
                os.write(b.getBytes());
                os.flush();
                System.out.println("SendPack end");
                if(wait>0)
                    return PullPack(wait, b.header.seq); //get result for the same seq
                else
                    return null;
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }

        private NetPack PullPack(int TimeOut, int seq) {
            long t = System.currentTimeMillis();
            while(System.currentTimeMillis() - t < TimeOut){
                if(mReceivePackQueue.size() == 0){
                    continue;
                }
                for(int i = mReceivePackQueue.size(); i > 0; i--) {
                    if (mReceivePackQueue.get(i - 1).header.seq == seq) {
                        NetPack b = mReceivePackQueue.get(i - 1);
                        synchronized (LockReceive) {
                            mReceivePackQueue.remove(i - 1);
                        }
                        DeleteFlag(seq);
                        return b;
                    }
                }
            }
            DeleteFlag(seq);
            return null;
        }

        private void DeleteFlag(int seq) {
            synchronized (LockWaitFlag) {
                if(mWaitFlagArr.contains(seq))
                    mWaitFlagArr.remove(Integer.valueOf(seq));
            }
        }

        public boolean CheckWaitingPack(NetPack p) {
            boolean isWait = false;
            if(mWaitFlagArr.size() == 0){
                return false;
            }
            for(int i = mWaitFlagArr.size(); i > 0; i--){
                if(p.header.seq == mWaitFlagArr.get(i - 1)){
                    synchronized (LockReceive) {
                        mReceivePackQueue.add(p);
                    }
                    isWait = true;
                    break;
                }
            }
            return isWait;
        }

        public Socket getSocket() { return s; }

        public PeerType getPT() { return PT; }

        public void addCmdHistory(String line) {
            cmdHistory.add(line);
        }

        public ArrayList<String> getCmdHistory() {
            return cmdHistory;
        }
    }
    final String TAG = "ConnMgr";
    ConcurrentHashMap<String, Connection> PeerList = new ConcurrentHashMap<>(); //UID to Connection
    public Connection.PeerType selfType;
    public String selfUIN = null;
    private static ConnMgr Instance = null;
    private ConnMgr() { }

    public static ConnMgr getInstance() {
        if(Instance == null)
            Instance = new ConnMgr();
        return Instance;
    }

    public ServerSocket makeServerSock(String m_host, int port) {
        InetAddress i;
        try {
            i = InetAddress.getByName(m_host);
        } catch (UnknownHostException e) {
            e.printStackTrace();
            return null;
        }
        //SocketFactory basicSocketFactory = SocketFactory.getDefault();
        try {
            //Socket s = basicSocketFactory.createSocket(i, port);
            ServerSocketFactory tlsSocketFactory = ServerSocketFactory.getDefault();
            ServerSocket s = tlsSocketFactory.createServerSocket(port);
            return s;
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public Connection makeConnection(String m_host, int port) {
        SocketFactory ssf = SocketFactory.getDefault();
        try {
            Socket s = ssf.createSocket(m_host, port);
            return new Connection(s, Connection.PeerType.SERVER);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }

    }

    public void doBroadcast(MsgPack msg) {
        for (Connection c : PeerList.values()) {
            msg.getMessage().msg_head.msg_seq = c.getNextSeq(); //set different seq for different peers.
            msg.getMessage().msg_head.to_uin = c.peerUin;
            NetPack np = msg.getNetPack(2); //broadcast msg type.
            if(!c.getSocket().isClosed())
                c.SendPack(np,0); //no reply needed.
        }
    }

    public void addNewPeer(String uid, Connection c) {
        PeerList.put(uid, c);
    }

    public void removePeer(String uid) {
        PeerList.remove(uid);
    }

    public Connection getPeer(String uid) {
        return PeerList.get(uid);
    }

    public Set<String> getPeerlist() {
        return PeerList.keySet();
    }

    public void setCfg(config rmCfg) {
        this.cfg = rmCfg;
    }

    public void startConn() {
        ConnMgr.Connection conn = makeConnection(cfg.getHost(), cfg.getPort());
        new Thread(new recvThread(conn)).start();
    }

}