package org.gtdev.webapps.deschat.network;

import com.qq.tars.protocol.tars.TarsInputStream;
import com.qq.tars.protocol.tars.TarsOutputStream;
import org.gtdev.webapps.deschat.network.DChatMsg.Msg;
import org.gtdev.webapps.deschat.network.DChatMsg.MsgType;

public class MsgPack {

    Msg message;
    ConnMgr.Connection connection = null;

    public MsgPack(NetPack p, ConnMgr.Connection c) {
        message = new Msg();
        connection = c;
        try {
            message.readFrom(new TarsInputStream(p.body));
        } catch (TarsInputStream.TarsDecodeException e) {
            e.printStackTrace();
        }
    }

    public MsgPack(Msg m) {
        this.message = m;
        message.msg_head.setMsg_time(System.currentTimeMillis());
    }

    public NetPack getNetPack(int type) {
        NetPack r = new NetPack();
        TarsOutputStream st = new TarsOutputStream();
        message.writeTo(st);
        r.body = st.toByteArray();
        r.header = NetPack.NetHeader.buildHeader((byte)1, (byte)0x21, (byte)0x12, (byte)type, (int)message.msg_head.msg_seq, r.body.length);
        return r;
    }

    public MsgPack DealCmd() {
        MsgType cmd = MsgType.convert(message.msg_head.msg_type);
        SvcHandler h = SvcHandler.EvDispatcher.getHandler(cmd.name());
        if(h == null)
            return null; //unable to handle
        //Record the cmd used by the client
        connection.addCmdHistory(cmd.name()+": "+message.msg_body);
        h.feedMsg(this);
        MsgPack t = h.reply();
        return t;
    }

    public Msg getMessage(){
        return message;
    }
    public ConnMgr.Connection getConnection () {
        return connection;
    }
}
