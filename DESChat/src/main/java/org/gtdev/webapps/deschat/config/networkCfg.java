package org.gtdev.webapps.deschat.config;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.gtdev.webapps.deschat.network.ConnMgr;

import javax.annotation.PostConstruct;

@Configuration
@EnableScheduling
public class networkCfg {
    @Autowired
    private ConnMgr cm;

    @Autowired
    private ConnMgr.config cfg;

    @PostConstruct
    public void conn_init() {
        cm.setCfg(cfg);
        cm.startConn();
    }

}
