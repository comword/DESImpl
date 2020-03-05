package org.gtdev.webapps.deschat.controllers;

import lombok.Getter;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

@Controller
public class DESController {

    static {
        System.loadLibrary("desjni");
    }

    @Getter
    static class DESReply {
        int status;
        String res;
    }

    @GetMapping(value = "/des/init", produces = "application/json")
    public @ResponseBody DESReply webDESInit(String key, String iv) {
        DESReply reply = new DESReply();
        if(!des_get_inited()) {
            boolean r = des_init(key, iv);
            reply.status = 0;
            if (r)
                reply.res = "Success";
            else
                reply.res = "Failed";
        } else {
            reply.status = -1;
            reply.res = "DES is already loaded.";
        }
        return reply;
    }

    @GetMapping(value = "/des/deinit", produces = "application/json")
    public @ResponseBody DESReply webDESdeinit() {
        DESReply reply = new DESReply();
        if(des_get_inited()) {
            des_deInit();
            reply.status = 0;
            reply.res = "Success";
        } else {
            reply.status = 0;
            reply.res = "DES is already unloaded.";
        }
        return reply;
    }

    @GetMapping(value = "/des/crypt", produces = "application/json")
    public @ResponseBody DESReply webDEScrypt(String content, boolean enc) {
        DESReply reply = new DESReply();
        if(!des_get_inited()) {
            reply.status = -1;
            reply.res = "DES is not loaded.";
        } else {
            reply.res = des_enc(content, enc);
            reply.status = 0;
        }
        return reply;
    }

    private native boolean des_init(String key, String iv);
    private native boolean des_get_inited();
    private native String des_enc(String content, boolean mode);
    private native void des_deInit();
    private native void des_fileEnc(String rFile, String oFile, boolean mode);
}
