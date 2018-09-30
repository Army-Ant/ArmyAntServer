/**
 * Created by Jason on 2016/5/31.
 */
"use strict";
import libArmyAnt from "./ArmyAnt.js/libArmyAnt.js"

let serverHost = {
    onStart: function () {
        let svr = new libArmyAnt.HttpServer();
        svr.start(8080);
    },

    onTest: function () {

    }
};

serverHost.onStart();