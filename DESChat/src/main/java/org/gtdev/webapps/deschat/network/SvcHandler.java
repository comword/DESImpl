package org.gtdev.webapps.deschat.network;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

public interface SvcHandler {
    void feedMsg(MsgPack fm);
    MsgPack reply();

    class EvDispatcher {
        private EvDispatcher() {}

        public static SvcHandler getHandler(String clsName) {
            Class<?> clazz = null;
            try {
                clazz = Class.forName("org.gtdev.webapps.deschat.network.functions."+clsName);
                Constructor<?> cons = clazz.getConstructor();
                SvcHandler obj = (SvcHandler) cons.newInstance();
                return obj;
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            } catch (NoSuchMethodException e) {
                e.printStackTrace();
            } catch (InstantiationException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            }
            return null;
        }
    }
}