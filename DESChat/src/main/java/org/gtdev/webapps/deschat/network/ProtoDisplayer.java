package org.gtdev.webapps.deschat.network;

import java.util.Map;

import com.qq.tars.protocol.tars.*;
import com.qq.tars.protocol.tars.TarsInputStream.HeadData;

public final class ProtoDisplayer {
    private StringBuilder	sb;
    private int				_level = 0;
    private TarsInputStream pis;

    private void ps(int tag, String fieldName) {
        for(int i = 0; i < _level; ++i)
            sb.append('\t');
        if(fieldName != null)
            sb.append(tag).append(',').append(fieldName).append(" ");
    }

    public ProtoDisplayer(StringBuilder sb, int level) {
        this.sb = sb;
        this._level = level;
    }

    public ProtoDisplayer(StringBuilder sb) {
        this.sb = sb;
    }

    public void display(TarsInputStream pis_) {
        this.pis = pis_;
        while(pis.getBs().hasRemaining()) {
            if(!do_display())
                break;
        }
    }

    private String getNamebyType(byte type) {
        switch(type) {
        case TarsStructBase.BYTE:
            return "BYTE:";
        case TarsStructBase.DOUBLE:
            return "DOUBLE:";
        case TarsStructBase.FLOAT:
            return "FLOAT:";
        case TarsStructBase.INT:
            return "INT:";
        case TarsStructBase.LIST:
            return "LIST:";
        case TarsStructBase.LONG:
            return "LONG:";
        case TarsStructBase.MAP:
            return "MAP:";
        case TarsStructBase.SHORT:
            return "SHORT:";
        case TarsStructBase.SIMPLE_LIST:
            return "SIMPLE_LIST:";
        case TarsStructBase.STRING1:
            return "STRING1:";
        case TarsStructBase.STRING4:
            return "STRING4:";
        case TarsStructBase.STRUCT_BEGIN:
            return "STRUCT_BEGIN:";
        case TarsStructBase.STRUCT_END:
            return "STRUCT_END";
        case TarsStructBase.ZERO_TAG:
            return "ZERO_TAG:";
        }
        return "UNKNOWN";
    }

    private boolean do_display() {
        HeadData hd = new HeadData();
        pis.peakHead(hd);
        ps(hd.tag, getNamebyType(hd.type));
        String s = "";
        switch( hd.type ) {
        case TarsStructBase.BYTE:
            byte a = 0;
            a = pis.read(a, hd.tag, false);
            sb.append(a).append('\n');
            break;
        case TarsStructBase.DOUBLE:
            double b = 0;
            b = pis.read(b, hd.tag, false);
            sb.append(b).append('\n');
            break;
        case TarsStructBase.FLOAT:
            float c = 0;
            c = pis.read(c, hd.tag, false);
            sb.append(c).append('\n');
            break;
        case TarsStructBase.INT:
            int d = 0;
            d = pis.read(d, hd.tag, false);
            sb.append(d).append('\n');
            break;
        case TarsStructBase.LIST:
            //pis.skipField();
        	String e[] = {""};
        	e = pis.read(e, hd.tag, false);
        	sb.append('[').append(e[0]);
        	for(int i=1; i<e.length; i++)
        		sb.append(", ").append(e[i]);
        	sb.append("]\n");
            break;
        case TarsStructBase.LONG:
            long f = 0;
            f = pis.read(f, hd.tag, false);
            sb.append(f).append('\n');
            break;
        case TarsStructBase.MAP:
            Map<String,String> m;
            m = pis.readStringMap(hd.tag, false);
            sb.append(m).append('\n');
            break;
        case TarsStructBase.SHORT:
            short h = 0;
            h = pis.read(h, hd.tag, false);
            sb.append(h).append('\n');
            break;
        case TarsStructBase.SIMPLE_LIST:
            byte[] i = null;
            i = pis.read(i, hd.tag, false);
            if ( i == null ) {
                sb.append("null\n");
                break;
            }
            if(i.length == 0) {
                sb.append("0, []\n");
            }
            sb.append(i.length).append(", [");
            sb.append(i[0]);
            for(int j=1; j<i.length; j++)
                sb.append(',').append(i[j]);
            sb.append("]\n");
            break;
        case TarsStructBase.STRING1:
            s = pis.read(s, hd.tag, false);
            sb.append(s).append('\n');
            break;
        case TarsStructBase.STRING4:
            s = pis.read(s, hd.tag, false);
            sb.append(s).append('\n');
            break;
        case TarsStructBase.STRUCT_BEGIN:
            sb.append('\n');
            pis.readHead(hd);
            StringBuilder sb2 = new StringBuilder();
            ProtoDisplayer jd = new ProtoDisplayer(sb2, _level + 1);
            jd.display(pis);
            sb.append(sb2.toString());
            //pis.skipField();
            break;
        case TarsStructBase.STRUCT_END:
            sb.append('\n');
            pis.skipToStructEnd();
            return false;
        case TarsStructBase.ZERO_TAG:
            sb.append("ZERO_TAG").append('\n');
            break;
        }
        return true;
    }

    @Override
    public String toString() {
        return sb.toString();
    }

}
