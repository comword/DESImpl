package org.gtdev.webapps.deschat.network;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class recvThread implements Runnable {

    private ConnMgr.Connection peer;
    private ConnMgr cm;

    public recvThread(ConnMgr.Connection s) {
        peer = s;
        cm = ConnMgr.getInstance();
    }

    @Override
    public void run() {
        NetPack p;
        //boolean isWaitPack = false;
        while(!peer.s.isClosed()) {
            try {
                p = PullPack();
                //isWaitPack = peer.CheckWaitingPack(p);
                if(peer.CheckWaitingPack(p))
                    continue; //The reply packet.
                MsgPack fm = new MsgPack(p, p.fromConnection);
                //check for past packet reply.
                if(fm.getMessage().msg_head.to_uin.equals(cm.selfUIN)){ //package for me
                    MsgPack repMsg = fm.DealCmd();
                    if(repMsg != null) {
                        NetPack repNet = repMsg.getNetPack((byte)1); //the reply packet's seq is same as received.
                        if(!peer.s.isClosed())
                            peer.SendPack(repNet,0);
                    }
                }
            } catch (IOException e) {
                //e.printStackTrace();
                try {
                    peer.s.close();
                } catch (IOException e1) {
                //    e1.printStackTrace();
                }
            }
        }
        System.out.println("Remote disconnected.");
        if(peer.peerUin != null)
            cm.removePeer(peer.peerUin);
        if(peer.getPT() == ConnMgr.Connection.PeerType.SERVER) {
            System.out.println("Server disconnected.");
            System.exit(0);
        }
    }

    public NetPack PullPack() throws IOException {
        InputStream in = peer.s.getInputStream();
        byte[] b_header = new byte[13]; // Packet head (1Byte)+ header data(12Byte)
        int len = 0;
        len = in.read(b_header);
        if(len != 13) throw new IOException();
        NetPack.NetHeader header = new NetPack.NetHeader(b_header);
        NetPack p = new NetPack();
        p.header = header;
        p.body = new byte[header.datalen+1];
        int readlen = 0, readed = 0;
        ByteBuffer buf = ByteBuffer.allocate(header.datalen+1);
        while(readed < header.datalen+1) {
            readlen = in.read(p.body, 0, header.datalen+1 - readlen);
            buf.put(p.body, 0, readlen);
            readed += readlen;
        }
        p.body = buf.array();
        p.fromConnection = peer;
        byte realtail = p.body[p.body.length-1];
        if(realtail != p.header.tail) //bad tail
            throw new IOException();
        return p;
    }

}
