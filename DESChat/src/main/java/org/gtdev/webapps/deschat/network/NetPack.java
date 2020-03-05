package org.gtdev.webapps.deschat.network;

import java.io.IOException;

public class NetPack {

    public static class NetHeader {
        public byte version;  //packet version :0
        public byte header;   //packet header :1
        public byte tail;     //packet tail :2
        public byte type;     //packet type :3
        public int seq;       //packet seq :4-7
        public int datalen;  //packet data length :8-11

        //default header 0x21, default tail 0x12

        private NetHeader() {}

        public NetHeader(byte[] b) throws IOException {
            byte h = b[0];
            version = b[1];
            header = b[2];
            tail = b[3];
            type = b[4];
            seq = utils.buf_to_int32(b,5);
            datalen = utils.buf_to_int32(b,9);
            if(h!=header)
                throw new IOException();
        }

        @Override
        public String toString() {
            return "head: "+header+" tail: "+tail+" type: "+type+" len: "+datalen;
        }

        public static NetHeader buildHeader(byte v, byte h, byte tail, byte type, int seq, int datalen) {
            NetHeader res = new NetHeader();
            res.version = v;
            res.header = h;
            res.tail = tail;
            res.type = type;
            res.seq = seq;
            res.datalen = datalen;
            return res;
        }

        public byte[] getBytes() {
            byte[] r = new byte[13];
            r[0] = header;
            r[1] = version;
            r[2] = header;
            r[3] = tail;
            r[4] = type;
            utils.int32_to_buf(r,5, seq);
            utils.int32_to_buf(r,9, datalen);
            return r;
        }
    }

    public NetHeader header;
    public byte[] body;
    public ConnMgr.Connection fromConnection;

    public byte[] getBytes() {
        byte[] a = header.getBytes();
        byte[] c = new byte[a.length + body.length + 1];
        System.arraycopy(a, 0, c, 0, a.length);
        System.arraycopy(body, 0, c, a.length, body.length);
        c[a.length + body.length] = header.tail;
        return c;
    }

}