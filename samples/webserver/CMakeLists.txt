if (${SAT_WEBSERVER} AND ${SAT_JSON})
    create_sample (sat_webserver_wildcard_sample webserver)
    create_sample (sat_webserver_static_sample webserver)
    
elseif (${SAT_WEBSERVER})
    create_sample (sat_webserver_static_sample webserver)


endif ()
