/*! Copyright (C) NAVER corp. Licensed under LGPL v2. @see https://github.com/naver/smarteditor2/blob/master/LICENSE.md */ ! function(i) {
    var n = {};

    function o(e) {
        if (n[e]) return n[e].exports;
        var t = n[e] = {
            i: e,
            l: !1,
            exports: {}
        };
        return i[e].call(t.exports, t, t.exports, o), t.l = !0, t.exports
    }
    o.m = i, o.c = n, o.d = function(e, t, i) {
        o.o(e, t) || Object.defineProperty(e, t, {
            enumerable: !0,
            get: i
        })
    }, o.r = function(e) {
        "undefined" != typeof Symbol && Symbol.toStringTag && Object.defineProperty(e, Symbol.toStringTag, {
            value: "Module"
        }), Object.defineProperty(e, "__esModule", {
            value: !0
        })
    }, o.t = function(t, e) {
        if (1 & e && (t = o(t)), 8 & e) return t;
        if (4 & e && "object" == typeof t && t && t.__esModule) return t;
        var i = Object.create(null);
        if (o.r(i), Object.defineProperty(i, "default", {
                enumerable: !0,
                value: t
            }), 2 & e && "string" != typeof t)
            for (var n in t) o.d(i, n, function(e) {
                return t[e]
            }.bind(null, n));
        return i
    }, o.n = function(e) {
        var t = e && e.__esModule ? function() {
            return e["default"]
        } : function() {
            return e
        };
        return o.d(t, "a", t), t
    }, o.o = function(e, t) {
        return Object.prototype.hasOwnProperty.call(e, t)
    }, o.p = "", o(o.s = 5)
}([, , , , , function(e, t, i) {
    "use strict";
    i(6), i(7), i(8), i(9), i(10), i(11), i(12), i(13), i(14), i(15), i(16), i(17), i(18), i(19), i(20), i(21), i(22), i(23), i(24), i(25), i(26), i(27), i(28), i(29), i(30), i(31), i(32), i(33), i(34), i(35), i(36), i(37), i(38), i(39), i(40), i(41), i(42), i(43), i(44), i(45), i(46), window.nSE2Version = "2.10.0.f24352b", nhn.husky.SE_EditingAreaManager.version = {
        revision: "f24352b",
        type: "open",
        number: "2.10.0"
    }
}, function(e, t) {
    nhn.husky.SE2M_Toolbar = jindo.$Class({
        name: "SE2M_Toolbar",
        toolbarArea: null,
        toolbarButton: null,
        uiNameTag: "uiName",
        nUIStatus: 1,
        sUIClassPrefix: "husky_seditor_ui_",
        aUICmdMap: null,
        elFirstToolbarItem: null,
        _hideAlert: function(e) {
            return "function" == typeof e && e(), this._elAlertLayer.style.display = "none", this.oApp.exec("HIDE_EDITING_AREA_COVER"), !1
        },
        $ON_ALERT: function(e, t) {
            this._elAlertLayer && (t = t || {}, this._elAlertTxts.innerHTML = e || "", this._elAlertOk.onclick = jindo.$Fn(this._hideAlert, this).bind(t.fOkCallback), this._elAlertClose.onclick = jindo.$Fn(this._hideAlert, this).bind(t.fCloseCallback), t.fCancelCallback ? (this._elAlertCancel.onclick = jindo.$Fn(this._hideAlert, this).bind(t.fCancelCallback), this._elAlertCancel.style.display = "") : this._elAlertCancel.style.display = "none", this.oApp.exec("SHOW_EDITING_AREA_COVER", [!0]), this._elAlertLayer.style.zIndex = 100, this._elAlertLayer.style.display = "block", this._elAlertOk.focus())
        },
        _assignHTMLElements: function(e) {
            e = jindo.$(e) || document, this.rxUI = new RegExp(this.sUIClassPrefix + "([^ ]+)"), this.toolbarArea = jindo.$$.getSingle(".se2_tool", e), this.aAllUI = jindo.$$("[class*=" + this.sUIClassPrefix + "]", this.toolbarArea), this.elTextTool = jindo.$$.getSingle("div.husky_seditor_text_tool", this.toolbarArea), this._elAlertLayer = jindo.$$.getSingle(".se2_alert_wrap", e), this._elAlertLayer && (this._elAlertTxts = jindo.$$.getSingle(".se2_alert_txts", this._elAlertLayer), this._elAlertOk = jindo.$$.getSingle(".se2_confirm", this._elAlertLayer), this._elAlertCancel = jindo.$$.getSingle(".se2_cancel", this._elAlertLayer), this._elAlertClose = jindo.$$.getSingle(".btn_close", this._elAlertLayer)), this.welToolbarArea = jindo.$Element(this.toolbarArea);
            for (var t = 0, i = this.aAllUI.length; t < i; t++)
                if (this.rxUI.test(this.aAllUI[t].className)) {
                    var n = RegExp.$1;
                    if (this.htUIList[n] !== undefined) continue;
                    this.htUIList[n] = this.aAllUI[t], this.htWrappedUIList[n] = jindo.$Element(this.htUIList[n])
                } null != jindo.$$.getSingle("DIV.se2_icon_tool") && (this.elFirstToolbarItem = jindo.$$.getSingle("DIV.se2_icon_tool UL.se2_itool1>li>button"))
        },
        $LOCAL_BEFORE_FIRST: function() {
            var e = jindo.$$(">ul>li[class*=" + this.sUIClassPrefix + "]>button", this.elTextTool),
                t = e.length;
            this.elFirstToolbarItem = this.elFirstToolbarItem || e[0], this.elLastToolbarItem = e[t - 1], this.oApp.registerBrowserEvent(this.toolbarArea, "keydown", "NAVIGATE_TOOLBAR", [])
        },
        $init: function(e, t) {
            this._htOptions = t || {}, this.htUIList = {}, this.htWrappedUIList = {}, this.aUICmdMap = {}, this._assignHTMLElements(e)
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.bMobile ? this.oApp.registerBrowserEvent(this.toolbarArea, "touchstart", "EVENT_TOOLBAR_TOUCHSTART") : (this.oApp.registerBrowserEvent(this.toolbarArea, "mouseover", "EVENT_TOOLBAR_MOUSEOVER"), this.oApp.registerBrowserEvent(this.toolbarArea, "mouseout", "EVENT_TOOLBAR_MOUSEOUT")), this.oApp.registerBrowserEvent(this.toolbarArea, "mousedown", "EVENT_TOOLBAR_MOUSEDOWN"), this.oApp.exec("ADD_APP_PROPERTY", ["getToolbarButtonByUIName", jindo.$Fn(this.getToolbarButtonByUIName, this).bind()]), this._htOptions.aDisabled && (this._htOptions._sDisabled = "," + this._htOptions.aDisabled.toString() + ",", this.oApp.exec("DISABLE_UI", [this._htOptions.aDisabled]))
        },
        $ON_NAVIGATE_TOOLBAR: function(e) {
            e.element == this.elLastToolbarItem && 9 == e.key().keyCode && (e.key().shift || (this.elFirstToolbarItem.focus(), e.stopDefault())), e.element == this.elFirstToolbarItem && 9 == e.key().keyCode && e.key().shift && (e.stopDefault(), this.elLastToolbarItem.focus())
        },
        $ON_TOGGLE_TOOLBAR_ACTIVE_LAYER: function(e, t, i, n, o, s) {
            this.oApp.exec("TOGGLE_ACTIVE_LAYER", [e, "MSG_TOOLBAR_LAYER_SHOWN", [e, t, i, n], o, s])
        },
        $ON_MSG_TOOLBAR_LAYER_SHOWN: function(e, t, i, n) {
            this.oApp.exec("POSITION_TOOLBAR_LAYER", [e, t]), i && this.oApp.exec(i, n)
        },
        $ON_SHOW_TOOLBAR_ACTIVE_LAYER: function(e, t, i, n) {
            this.oApp.exec("SHOW_ACTIVE_LAYER", [e, t, i]), this.oApp.exec("POSITION_TOOLBAR_LAYER", [e, n])
        },
        $ON_ENABLE_UI: function(e) {
            this._enableUI(e)
        },
        $ON_DISABLE_UI: function(e) {
            if (e instanceof Array)
                for (var t, i = 0; t = e[i]; i++) this._disableUI(t);
            else this._disableUI(e)
        },
        $ON_SELECT_UI: function(e) {
            var t = this.htWrappedUIList[e];
            t && (t.removeClass("hover"), t.addClass("active"))
        },
        $ON_DESELECT_UI: function(e) {
            var t = this.htWrappedUIList[e];
            t && t.removeClass("active")
        },
        $ON_TOGGLE_UI_SELECTED: function(e) {
            var t = this.htWrappedUIList[e];
            t && (t.hasClass("active") ? t.removeClass("active") : (t.removeClass("hover"), t.addClass("active")))
        },
        $ON_ENABLE_ALL_UI: function(e) {
            if (1 !== this.nUIStatus) {
                var t;
                e = e || {};
                var i = jindo.$A(e.aExceptions || []);
                for (t in this.htUIList) t && !i.has(t) && this._enableUI(t);
                this.nUIStatus = 1
            }
        },
        $ON_DISABLE_ALL_UI: function(e) {
            if (2 !== this.nUIStatus) {
                var t;
                e = e || {};
                var i = jindo.$A(e.aExceptions || []),
                    n = e.bLeaveActiveLayer || !1;
                for (t in n || this.oApp.exec("HIDE_ACTIVE_LAYER"), this.htUIList) t && !i.has(t) && this._disableUI(t);
                this.nUIStatus = 2
            }
        },
        $ON_MSG_STYLE_CHANGED: function(e, t) {
            "@^" === t ? this.oApp.exec("SELECT_UI", [e]) : this.oApp.exec("DESELECT_UI", [e])
        },
        $ON_POSITION_TOOLBAR_LAYER: function(e, t) {
            var i, n, o, s;
            e = jindo.$(e), t = t || {};
            var r = jindo.$(t.elBtn),
                a = t.sAlign;
            if (e) {
                r && r.tagName && "BUTTON" == r.tagName && r.parentNode.appendChild(e);
                var l = jindo.$Element(e);
                "right" != a ? (e.style.left = "0", n = (i = l.offset().left) + e.offsetWidth, (s = (o = this.welToolbarArea.offset().left) + this.toolbarArea.offsetWidth) < n && l.css("left", s - n - -1 + "px"), i < o && l.css("left", o - i - 1 + "px")) : (e.style.right = "0", n = (i = l.offset().left) + e.offsetWidth, (s = (o = this.welToolbarArea.offset().left) + this.toolbarArea.offsetWidth) < n && l.css("right", -1 * (s - n - -1) + "px"), i < o && l.css("right", -1 * (o - i - 1) + "px"))
            }
        },
        $ON_EVENT_TOOLBAR_MOUSEOVER: function(e) {
            if (2 !== this.nUIStatus)
                for (var t = this._getAffectedElements(e.element), i = 0; i < t.length; i++) t[i].hasClass("active") || t[i].addClass("hover")
        },
        $ON_EVENT_TOOLBAR_MOUSEOUT: function(e) {
            if (2 !== this.nUIStatus)
                for (var t = this._getAffectedElements(e.element), i = 0; i < t.length; i++) t[i].removeClass("hover")
        },
        $ON_EVENT_TOOLBAR_MOUSEDOWN: function(e) {
            for (var t = e.element; t;) {
                if (t.className && t.className.match(/active/) && (2 < t.childNodes.length || t.parentNode.className.match(/se2_pair/))) return;
                t = t.parentNode
            }
            this.oApp.exec("HIDE_ACTIVE_LAYER_IF_NOT_CHILD", [e.element])
        },
        _enableUI: function(e) {
            if (!(this._htOptions._sDisabled && -1 < this._htOptions._sDisabled.indexOf("," + e + ","))) {
                var t, i;
                this.nUIStatus = 0;
                var n = this.htWrappedUIList[e],
                    o = this.htUIList[e];
                if (n) {
                    n.removeClass("off");
                    var s = o.getElementsByTagName("BUTTON");
                    for (t = 0, i = s.length; t < i; t++) s[t].disabled = !1;
                    var r = "";
                    if (this.aUICmdMap[e])
                        for (t = 0; t < this.aUICmdMap[e].length; t++) r = this.aUICmdMap[e][t], this.oApp.exec("ENABLE_MESSAGE", [r])
                }
            }
        },
        _disableUI: function(e) {
            var t, i;
            this.nUIStatus = 0;
            var n = this.htWrappedUIList[e],
                o = this.htUIList[e];
            if (n) {
                n.addClass("off"), n.removeClass("hover");
                var s = o.getElementsByTagName("BUTTON");
                for (t = 0, i = s.length; t < i; t++) s[t].disabled = !0;
                var r = "";
                if (this.aUICmdMap[e])
                    for (t = 0; t < this.aUICmdMap[e].length; t++) r = this.aUICmdMap[e][t], this.oApp.exec("DISABLE_MESSAGE", [r])
            }
        },
        _getAffectedElements: function(e) {
            var t, i;
            if (!e.bSE2_MDCancelled) {
                e.bSE2_MDCancelled = !0;
                for (var n = e.getElementsByTagName("BUTTON"), o = 0, s = n.length; o < s; o++) n[o].onmousedown = function() {
                    return !1
                }
            }
            if (!e || !e.tagName) return [];
            if ("BUTTON" == (t = e).tagName) return (t = t.parentNode) && "LI" == t.tagName && this.rxUI.test(t.className) ? [jindo.$Element(t)] : (t = (t = e).parentNode.parentNode) && "LI" == t.tagName && (i = jindo.$Element(t)).hasClass("se2_pair") ? [i, jindo.$Element(e.parentNode)] : [];
            if ("SPAN" == (t = e).tagName) {
                if ((t = t.parentNode.parentNode) && "LI" == t.tagName && this.rxUI.test(t.className)) return [jindo.$Element(t)];
                if ((t = t.parentNode) && "LI" == t.tagName && this.rxUI.test(t.className)) return [jindo.$Element(t)]
            }
            return []
        },
        $ON_REGISTER_UI_EVENT: function(e, t, i, n) {
            var o;
            this.htUIList[e] && (this.aUICmdMap[e] || (this.aUICmdMap[e] = []), this.aUICmdMap[e][this.aUICmdMap[e].length] = i, (o = jindo.$$.getSingle("button", this.htUIList[e])) && this.oApp.registerBrowserEvent(o, t, i, n))
        },
        getToolbarButtonByUIName: function(e) {
            return jindo.$$.getSingle("BUTTON", this.htUIList[e])
        }
    })
}, function(e, t) {
    nhn.husky.SE_EditingAreaManager = jindo.$Class({
        name: "SE_EditingAreaManager",
        oActivePlugin: null,
        elContentsField: null,
        bIsDirty: !1,
        bAutoResize: !1,
        $init: function(e, t, i, n, o) {
            this.sDefaultEditingMode = e, this.elContentsField = jindo.$(t), this._assignHTMLElements(o), this.fOnBeforeUnload = n, this.oEditingMode = {}, this.elContentsField.style.display = "none", this.nMinWidth = parseInt(i.nMinWidth || 60, 10), this.nMinHeight = parseInt(i.nMinHeight || 60, 10);
            var s = this._getSize([i.nWidth, i.width, this.elEditingAreaContainer.offsetWidth], this.nMinWidth),
                r = this._getSize([i.nHeight, i.height, this.elEditingAreaContainer.offsetHeight], this.nMinHeight);
            this.elEditingAreaContainer.style.width = s.nSize + s.sUnit, this.elEditingAreaContainer.style.height = r.nSize + r.sUnit, "px" === s.sUnit ? o.style.width = s.nSize + 2 + "px" : "%" === s.sUnit && (o.style.minWidth = this.nMinWidth + "px")
        },
        _getSize: function(e, t) {
            var i, n, o, s, r, a = "px";
            for (t = parseInt(t, 10), i = 0, n = e.length; i < n; i++)
                if (e[i]) {
                    if (!isNaN(e[i])) {
                        s = parseInt(e[i], 10), r = a;
                        break
                    }
                    if (!(!(o = /([0-9]+)(.*)/i.exec(e[i])) || o.length < 2 || o[1] <= 0)) {
                        s = parseInt(o[1], 10), r = (r = o[2]) || a, s < t && r === a && (s = t);
                        break
                    }
                } return r = r || a, (isNaN(s) || s < t && r === a) && (s = t), {
                nSize: s,
                sUnit: r
            }
        },
        _assignHTMLElements: function(e) {
            this.elEditingAreaContainer = jindo.$$.getSingle("DIV.husky_seditor_editing_area_container", e);

            function t() {}
            this.elLoadingLayer = jindo.$$.getSingle(".se2_content_loading", e), this.elLoadingLayer || (this.$ON_SHOW_LOADING_LAYER = t, this.$ON_HIDE_LOADING_LAYER = t)
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["version", nhn.husky.SE_EditingAreaManager.version]), this.oApp.exec("ADD_APP_PROPERTY", ["elEditingAreaContainer", this.elEditingAreaContainer]), this.oApp.exec("ADD_APP_PROPERTY", ["welEditingAreaContainer", jindo.$Element(this.elEditingAreaContainer)]), this.oApp.exec("ADD_APP_PROPERTY", ["getEditingAreaHeight", jindo.$Fn(this.getEditingAreaHeight, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["getEditingAreaWidth", jindo.$Fn(this.getEditingAreaWidth, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["getRawContents", jindo.$Fn(this.getRawContents, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["getContents", jindo.$Fn(this.getContents, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["getIR", jindo.$Fn(this.getIR, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["setContents", this.setContents]), this.oApp.exec("ADD_APP_PROPERTY", ["setIR", this.setIR]), this.oApp.exec("ADD_APP_PROPERTY", ["getEditingMode", jindo.$Fn(this.getEditingMode, this).bind()])
        },
        $ON_MSG_APP_READY: function() {
            this.htOptions = this.oApp.htOptions[this.name] || {}, this.sDefaultEditingMode = this.htOptions.sDefaultEditingMode || this.sDefaultEditingMode, this.iframeWindow = this.oApp.getWYSIWYGWindow(), this.oApp.exec("REGISTER_CONVERTERS", []), this.oApp.exec("CHANGE_EDITING_MODE", [this.sDefaultEditingMode, !0]), this.oApp.exec("LOAD_CONTENTS_FIELD", [!1]), !1 !== this.fOnBeforeUnload && (this.fOnBeforeUnload ? window.onbeforeunload = this.fOnBeforeUnload : window.onbeforeunload = jindo.$Fn(function() {
                if (this.getRawContents() != this.sCurrentRawContents || this.bIsDirty) return this.oApp.$MSG("SE_EditingAreaManager.onExit")
            }, this).bind())
        },
        $AFTER_MSG_APP_READY: function() {
            this.oApp.exec("UPDATE_RAW_CONTENTS"), this.oApp.htOptions[this.name] && this.oApp.htOptions[this.name].bAutoResize && (this.bAutoResize = this.oApp.htOptions[this.name].bAutoResize), this.oApp.oNavigator.msafari && (this.bAutoResize = !0), this.startAutoResize()
        },
        $ON_LOAD_CONTENTS_FIELD: function(e) {
            var t = this.elContentsField.value;
            t = t.replace(/^\s+/, ""), this.oApp.exec("SET_CONTENTS", [t, e])
        },
        $ON_UPDATE_CONTENTS_FIELD: function() {
            this.elContentsField.value = this.oApp.getContents(), this.oApp.exec("UPDATE_RAW_CONTENTS")
        },
        $ON_UPDATE_RAW_CONTENTS: function() {
            this.sCurrentRawContents = this.oApp.getRawContents()
        },
        $BEFORE_CHANGE_EDITING_MODE: function(e) {
            if (!this.oEditingMode[e]) return !1;
            this.stopAutoResize(), this._oPrevActivePlugin = this.oActivePlugin, this.oActivePlugin = this.oEditingMode[e]
        },
        $AFTER_CHANGE_EDITING_MODE: function(e, t) {
            if (this._oPrevActivePlugin) {
                var i = this._oPrevActivePlugin.getIR();
                this.oApp.exec("SET_IR", [i]), this._setEditingAreaDimension()
            }
            this.startAutoResize(), t || this.oApp.delayedExec("FOCUS", [], 0)
        },
        $ON_SET_IS_DIRTY: function(e) {
            this.bIsDirty = e
        },
        $ON_FOCUS: function(e) {
            this.oActivePlugin && "function" == typeof this.oActivePlugin.setIR && (this.iframeWindow && this.iframeWindow.document.hasFocus && !this.iframeWindow.document.hasFocus() && "WYSIWYG" == this.oActivePlugin.sMode ? this.iframeWindow.focus() : this.oActivePlugin.focus(), e && this.oApp.bMobile || this.oActivePlugin.focus())
        },
        $ON_IE_FOCUS: function() {
            var e = this.oApp.oNavigator;
            (e.ie || e.edge) && this.oApp.exec("FOCUS")
        },
        $ON_SET_CONTENTS: function(e, t) {
            this.setContents(e, t)
        },
        $BEFORE_SET_IR: function(e, t) {
            (t = t || !1) || this.oApp.exec("RECORD_UNDO_ACTION", ["BEFORE SET CONTENTS", {
                sSaveTarget: "BODY"
            }])
        },
        $ON_SET_IR: function(e) {
            this.oActivePlugin && "function" == typeof this.oActivePlugin.setIR && this.oActivePlugin.setIR(e)
        },
        $AFTER_SET_IR: function(e, t) {
            (t = t || !1) || this.oApp.exec("RECORD_UNDO_ACTION", ["AFTER SET CONTENTS", {
                sSaveTarget: "BODY"
            }])
        },
        $ON_REGISTER_EDITING_AREA: function(e) {
            "WYSIWYG" == (this.oEditingMode[e.sMode] = e).sMode && this.attachDocumentEvents(e.oEditingArea), this._setEditingAreaDimension(e)
        },
        $ON_MSG_EDITING_AREA_RESIZE_STARTED: function() {
            this._fitElementInEditingArea(this.elEditingAreaContainer), this.oApp.exec("STOP_AUTORESIZE_EDITING_AREA"), this.oApp.exec("SHOW_EDITING_AREA_COVER"), this.elEditingAreaContainer.style.overflow = "hidden", this.iStartingHeight = parseInt(this.elEditingAreaContainer.style.height, 10)
        },
        $ON_STOP_AUTORESIZE_EDITING_AREA: function() {
            this.bAutoResize && (this.stopAutoResize(), this.bAutoResize = !1)
        },
        startAutoResize: function() {
            this.bAutoResize && this.oActivePlugin && "function" == typeof this.oActivePlugin.startAutoResize && this.oActivePlugin.startAutoResize()
        },
        stopAutoResize: function() {
            this.bAutoResize && this.oActivePlugin && "function" == typeof this.oActivePlugin.stopAutoResize && this.oActivePlugin.stopAutoResize()
        },
        $ON_RESIZE_EDITING_AREA: function(e, t) {
            null != e && this._resizeWidth(e, "px"), null != t && this._resizeHeight(t, "px"), this._setEditingAreaDimension()
        },
        _resizeWidth: function(e, t) {
            var i = parseInt(e, 10);
            i < this.nMinWidth && (i = this.nMinWidth), e && (this.elEditingAreaContainer.style.width = i + t)
        },
        _resizeHeight: function(e, t) {
            var i = parseInt(e, 10);
            i < this.nMinHeight && (i = this.nMinHeight), e && (this.elEditingAreaContainer.style.height = i + t)
        },
        $ON_RESIZE_EDITING_AREA_BY: function(e, t) {
            var i, n, o = parseInt(e, 10),
                s = parseInt(t, 10);
            0 !== e && -1 === this.elEditingAreaContainer.style.width.indexOf("%") && (i = this.elEditingAreaContainer.style.width ? parseInt(this.elEditingAreaContainer.style.width, 10) + o : null), 0 !== s && (n = this.elEditingAreaContainer.style.height ? this.iStartingHeight + s : null), (e || s) && this.oApp.exec("RESIZE_EDITING_AREA", [i, n])
        },
        $ON_MSG_EDITING_AREA_RESIZE_ENDED: function() {
            this.oApp.exec("HIDE_EDITING_AREA_COVER"), this.elEditingAreaContainer.style.overflow = "", this._setEditingAreaDimension()
        },
        $ON_SHOW_EDITING_AREA_COVER: function(e) {
            this.elEditingAreaCover || (this.elEditingAreaCover = document.createElement("DIV"), this.elEditingAreaCover.style.cssText = "position:absolute;top:0;left:0;z-index:100;background:#000000;filter:alpha(opacity=0);opacity:0.0;-moz-opacity:0.0;-khtml-opacity:0.0;height:100%;width:100%", this.elEditingAreaContainer.appendChild(this.elEditingAreaCover)), e && jindo.$Element(this.elEditingAreaCover).opacity(.4), this.elEditingAreaCover.style.display = "block"
        },
        $ON_HIDE_EDITING_AREA_COVER: function() {
            this.elEditingAreaCover && (this.elEditingAreaCover.style.display = "none", jindo.$Element(this.elEditingAreaCover).opacity(0))
        },
        $ON_KEEP_WITHIN_EDITINGAREA: function(e, t) {
            var i = parseInt(e.style.top, 10);
            i + e.offsetHeight > this.oApp.elEditingAreaContainer.offsetHeight && (e.style.top = "number" == typeof t ? i - e.offsetHeight - t + "px" : this.oApp.elEditingAreaContainer.offsetHeight - e.offsetHeight + "px"), parseInt(e.style.left, 10) + e.offsetWidth > this.oApp.elEditingAreaContainer.offsetWidth && (e.style.left = this.oApp.elEditingAreaContainer.offsetWidth - e.offsetWidth + "px")
        },
        $ON_EVENT_EDITING_AREA_KEYDOWN: function() {
            this.oApp.exec("HIDE_ACTIVE_LAYER", [])
        },
        $ON_EVENT_EDITING_AREA_MOUSEDOWN: function() {
            this.oApp.exec("HIDE_ACTIVE_LAYER", [])
        },
        $ON_EVENT_EDITING_AREA_SCROLL: function() {
            this.oApp.exec("HIDE_ACTIVE_LAYER", [])
        },
        _setEditingAreaDimension: function(e) {
            e = e || this.oActivePlugin, this._fitElementInEditingArea(e.elEditingArea)
        },
        _fitElementInEditingArea: function(e) {
            e.style.height = this.elEditingAreaContainer.offsetHeight + "px"
        },
        attachDocumentEvents: function(e) {
            this.oApp.registerBrowserEvent(e, "click", "EVENT_EDITING_AREA_CLICK"), this.oApp.registerBrowserEvent(e, "dblclick", "EVENT_EDITING_AREA_DBLCLICK"), this.oApp.registerBrowserEvent(e, "mousedown", "EVENT_EDITING_AREA_MOUSEDOWN"), this.oApp.registerBrowserEvent(e, "mousemove", "EVENT_EDITING_AREA_MOUSEMOVE"), this.oApp.registerBrowserEvent(e, "mouseup", "EVENT_EDITING_AREA_MOUSEUP"), this.oApp.registerBrowserEvent(e, "mouseout", "EVENT_EDITING_AREA_MOUSEOUT"), this.oApp.registerBrowserEvent(e, "mousewheel", "EVENT_EDITING_AREA_MOUSEWHEEL"), this.oApp.registerBrowserEvent(e, "keydown", "EVENT_EDITING_AREA_KEYDOWN"), this.oApp.registerBrowserEvent(e, "keypress", "EVENT_EDITING_AREA_KEYPRESS"), this.oApp.registerBrowserEvent(e, "keyup", "EVENT_EDITING_AREA_KEYUP"), this.oApp.registerBrowserEvent(e, "scroll", "EVENT_EDITING_AREA_SCROLL")
        },
        $ON_GET_COVER_DIV: function(e, t) {
            this.elEditingAreaCover && (t[e] = this.elEditingAreaCover)
        },
        $ON_SHOW_LOADING_LAYER: function() {
            this.elLoadingLayer.style.display = "block"
        },
        $ON_HIDE_LOADING_LAYER: function() {
            this.elLoadingLayer.style.display = "none"
        },
        getIR: function() {
            return this.oActivePlugin ? this.oActivePlugin.getIR() : ""
        },
        setIR: function(e, t) {
            this.oApp.exec("SET_IR", [e, t])
        },
        getRawContents: function() {
            return this.oActivePlugin ? this.oActivePlugin.getRawContents() : ""
        },
        getContents: function() {
            this._convertLastBrToNbsp();
            var e, t = this.oApp.getIR();
            return e = this.oApp.applyConverter ? this.oApp.applyConverter("IR_TO_DB", t, this.oApp.getWYSIWYGDocument()) : t, e = this._cleanContents(e)
        },
        _convertLastBrToNbsp: function() {
            for (var e, t, i, n, o, s, r, a, l, h, d, c = this.oApp.getWYSIWYGDocument().body, p = document.createTextNode(" "), u = 0, _ = (e = jindo.$$("br:last-child", c, {
                    oneTimeOffCache: !0
                })).length; u < _; u++)
                if (d = s = null, t = e[u], (n = this._findNextSiblingRecursive(t, {
                        isReverse: !0
                    })) && 1 === n.nodeType && (i = n.nextSibling, !this._findNextSiblingRecursive(i))) {
                    if ("IMG" === n.tagName.toUpperCase() ? s = n : 0 < (o = jindo.$$("img:last-child", n, {
                            oneTimeOffCache: !0
                        })).length && (s = o[o.length - 1]), s)
                        if (this._findNextSiblingRecursive(s) == i) {
                            if (!(a = s.parentNode)) continue;
                            r = jindo.$Element(s).width(), a = s.parentNode, l = !(r === jindo.$Element(a).width())
                        } else l = !0;
                    else l = !0;
                    d = t.parentNode, l ? (h = p.cloneNode(!1), d.replaceChild(h, t)) : this._recursiveRemoveChild(t)
                }
        },
        _findNextSiblingRecursive: function(e, t) {
            for (var i, n = e, o = !(!t || !t.isReverse), s = new RegExp("^(TD|BODY)$", "i"); !i;)
                if (i = o ? n.previousSibling : n.nextSibling, n = n.parentNode, s.test(n.tagName)) {
                    i = null;
                    break
                } return i
        },
        _recursiveRemoveChild: function(e) {
            for (var t, i = e.parentNode, n = e; i.removeChild(n), (t = i.childNodes) && 0 == t.length && (n = i) && (i = i.parentNode););
        },
        _cleanContents: function(e) {
            return e.replace(new RegExp("(<img [^>]*>)" + unescape("%uFEFF"), "ig"), "$1")
        },
        setContents: function(e, t) {
            var i;
            i = this.oApp.applyConverter ? this.oApp.applyConverter("DB_TO_IR", e, this.oApp.getWYSIWYGDocument()) : e, this.oApp.exec("SET_IR", [i, t])
        },
        getEditingMode: function() {
            return this.oActivePlugin.sMode
        },
        getEditingAreaWidth: function() {
            return this.elEditingAreaContainer.offsetWidth
        },
        getEditingAreaHeight: function() {
            return this.elEditingAreaContainer.offsetHeight
        }
    })
}, function(e, t) {
    nhn.husky.SE_EditingArea_WYSIWYG = jindo.$Class({
        name: "SE_EditingArea_WYSIWYG",
        status: nhn.husky.PLUGIN_STATUS.NOT_READY,
        sMode: "WYSIWYG",
        iframe: null,
        doc: null,
        bStopCheckingBodyHeight: !1,
        bAutoResize: !1,
        nBodyMinHeight: 0,
        nScrollbarWidth: 0,
        iLastUndoRecorded: 0,
        _nIFrameReadyCount: 50,
        bWYSIWYGEnabled: !1,
        $init: function(e) {
            this.iframe = jindo.$(e);
            var t = jindo.$Agent().navigator();
            t.ie && (this.iframe.style.display = "none"), this.sBlankPageURL = "smart_editor2_inputarea.html", this.sBlankPageURL_EmulateIE7 = "smart_editor2_inputarea_ie8.html", this.aAddtionalEmulateIE7 = [], this.htOptions = nhn.husky.SE2M_Configuration.SE_EditingAreaManager, this.htOptions && (this.sBlankPageURL = this.htOptions.sBlankPageURL || this.sBlankPageURL, this.sBlankPageURL_EmulateIE7 = this.htOptions.sBlankPageURL_EmulateIE7 || this.sBlankPageURL_EmulateIE7, this.aAddtionalEmulateIE7 = this.htOptions.aAddtionalEmulateIE7 || this.aAddtionalEmulateIE7), this.aAddtionalEmulateIE7.push(8), this.sIFrameSrc = this.sBlankPageURL, t.ie && jindo.$A(this.aAddtionalEmulateIE7).has(t.nativeVersion) && (this.sIFrameSrc = this.sBlankPageURL_EmulateIE7), e = this.iframe;
            var i = this.sIFrameSrc,
                n = jindo.$Fn(this.initIframe, this).bind(),
                o = jindo.$Fn(function() {
                    this.iframe.src = i
                }, this).bind();
            !t.ie || 9 <= t.version && document.addEventListener ? (e.addEventListener("load", n, !1), e.addEventListener("error", o, !1)) : (e.attachEvent("onload", n), e.attachEvent("onerror", o)), e.src = i, this.elEditingArea = e
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oEditingArea = this.iframe.contentWindow.document, this.oApp.exec("REGISTER_EDITING_AREA", [this]), this.oApp.exec("ADD_APP_PROPERTY", ["getWYSIWYGWindow", jindo.$Fn(this.getWindow, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["getWYSIWYGDocument", jindo.$Fn(this.getDocument, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["isWYSIWYGEnabled", jindo.$Fn(this.isWYSIWYGEnabled, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["getRawHTMLContents", jindo.$Fn(this.getRawHTMLContents, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["setRawHTMLContents", jindo.$Fn(this.setRawHTMLContents, this).bind()]), this.isWYSIWYGEnabled() && this.oApp.exec("ENABLE_WYSIWYG_RULER"), this.oApp.registerBrowserEvent(this.getDocument().body, "paste", "EVENT_EDITING_AREA_PASTE"), this.oApp.registerBrowserEvent(this.getDocument().body, "drop", "EVENT_EDITING_AREA_DROP")
        },
        $ON_MSG_APP_READY: function() {
            Object.prototype.hasOwnProperty.call(this.oApp, "saveSnapShot") || (this.$ON_EVENT_EDITING_AREA_MOUSEUP = function() {}, this._recordUndo = function() {}), this._bIERangeReset = !0, this.oApp.oNavigator.ie || -1 < navigator.userAgent.indexOf("Edge") ? (this._bIECursorHide = !0, jindo.$Fn(function(e) {
                var t = this.iframe.contentWindow.document.selection;
                t && "control" === t.type.toLowerCase() && 8 === e.key().keyCode && (this.oApp.exec("EXECCOMMAND", ["delete", !1, !1]), e.stop()), this._bIERangeReset = !1
            }, this).attach(this.iframe.contentWindow.document, "keydown"), jindo.$Fn(function() {
                this._oIERange = null, this._bIERangeReset = !0
            }, this).attach(this.iframe.contentWindow.document.body, "mousedown"), this.getDocument().createRange || jindo.$Fn(this._onIEBeforeDeactivate, this).attach(this.iframe.contentWindow.document.body, "beforedeactivate"), jindo.$Fn(function() {
                this._bIERangeReset = !1
            }, this).attach(this.iframe.contentWindow.document.body, "mouseup")) : this.oApp.oNavigator.bGPadBrowser && (this.$ON_EVENT_TOOLBAR_TOUCHSTART = function() {
                this._oIERange = this.oApp.getSelection().cloneRange()
            }), this.fnSetBodyHeight = jindo.$Fn(this._setBodyHeight, this).bind(), this.fnCheckBodyChange = jindo.$Fn(this._checkBodyChange, this).bind(), this.fnSetBodyHeight(), this._nContainerHeight = this.oApp.getEditingAreaHeight(), this._setScrollbarWidth()
        },
        $ON_REGISTER_CONVERTERS: function() {
            this.oApp.exec("ADD_CONVERTER_DOM", ["DB_TO_IR", jindo.$Fn(this._dbToIrDOM, this).bind()])
        },
        _dbToIrDOM: function(e) {
            nhn.husky.SE2M_Utils.removeInvalidFont(e), nhn.husky.SE2M_Utils.convertFontToSpan(e)
        },
        _setScrollbarWidth: function() {
            var e = this.getDocument(),
                t = e.createElement("div");
            t.style.width = "100px", t.style.height = "100px", t.style.overflow = "scroll", t.style.position = "absolute", t.style.top = "-9999px", e.body.appendChild(t), this.nScrollbarWidth = t.offsetWidth - t.clientWidth, e.body.removeChild(t)
        },
        $AFTER_EVENT_EDITING_AREA_KEYUP: function(e) {
            if (this.bAutoResize) {
                var t = e.key();
                33 <= t.keyCode && t.keyCode <= 40 || t.alt || t.ctrl || 16 === t.keyCode || this._setAutoResize()
            }
        },
        $AFTER_PASTE_HTML: function() {
            this.bAutoResize && this._setAutoResize()
        },
        startAutoResize: function() {
            this.oApp.exec("STOP_CHECKING_BODY_HEIGHT"), this.bAutoResize = !0;
            var e = this.oApp.oNavigator;
            e.ie && e.version < 9 ? jindo.$Element(this.getDocument().body).css({
                overflow: "visible"
            }) : jindo.$Element(this.getDocument().body).css({
                overflowX: "visible",
                overflowY: "hidden"
            }), this._setAutoResize(), this.nCheckBodyInterval = setInterval(this.fnCheckBodyChange, 500), this.oApp.exec("START_FLOAT_TOOLBAR")
        },
        stopAutoResize: function() {
            this.bAutoResize = !1, clearInterval(this.nCheckBodyInterval), this.oApp.exec("STOP_FLOAT_TOOLBAR"), jindo.$Element(this.getDocument().body).css({
                overflow: "visible",
                overflowY: "visible"
            }), this.oApp.exec("START_CHECKING_BODY_HEIGHT")
        },
        _checkBodyChange: function() {
            if (this.bAutoResize) {
                var e = this.getDocument().body.innerHTML.length;
                e !== this.nBodyLength && (this.nBodyLength = e, this._setAutoResize())
            }
        },
        _setAutoResize: function() {
            var e, t, i, n, o = this.getDocument().body,
                s = jindo.$Element(o),
                r = !1,
                a = this.oApp.oNavigator;
            this.nTopBottomMargin = this.nTopBottomMargin || parseInt(s.css("marginTop"), 10) + parseInt(s.css("marginBottom"), 10), this.nBodyMinHeight = this.nBodyMinHeight || this.oApp.getEditingAreaHeight() - this.nTopBottomMargin, s.css("height", "0px"), this.iframe.style.height = "0px", (e = parseInt(o.scrollHeight, 10)) < this.nBodyMinHeight && (e = this.nBodyMinHeight), a.ie ? e > this.nBodyMinHeight ? (i = this.oApp.getCurrentStyle(), (n = this._getStyleSize(i)) < this.nTopBottomMargin && (n = this.nTopBottomMargin), t = e + n, t += 18, r = !0) : (e = this.nBodyMinHeight, t = this.nBodyMinHeight + this.nTopBottomMargin) : e > this.nBodyMinHeight ? (i = this.oApp.getCurrentStyle(), (n = this._getStyleSize(i)) < this.nTopBottomMargin && (n = this.nTopBottomMargin), t = e + n, r = !0) : (e = this.nBodyMinHeight, t = this.nBodyMinHeight + this.nTopBottomMargin), s.css("height", e + "px"), this.iframe.style.height = t + "px", this.oApp.welEditingAreaContainer.height(t), this._nContainerHeight !== t && (this._nContainerHeight = t, this.oApp.exec("MSG_EDITING_AREA_SIZE_CHANGED")), this.oApp.oNavigator.msafari || this.oApp.checkResizeGripPosition(r)
        },
        _getStyleSize: function(e) {
            var t;
            if (e) {
                var i = e.lineHeight;
                if (i && /[^\d.]/.test(i))
                    if (/\d/.test(i) && /[A-Za-z]/.test(i)) {
                        if (/px$/.test(i)) return parseFloat(i, 10);
                        if (/pt$/.test(i)) return 4 * parseFloat(i, 10) / 3;
                        if (/em$/.test(i)) return 16 * parseFloat(i, 10);
                        if (/cm$/.test(i)) return 96 * parseFloat(i, 10) / 2.54
                    } else /\d/.test(i) && /%/.test(i) ? i = 100 * parseFloat(i, 10) : /[^A-Za-z]/.test(i) || (i = 1.2);
                var n = e.fontSize;
                n && !/px$/.test(n) && (n = /pt$/.test(n) ? 4 * parseFloat(n, 10) / 3 + "px" : /em$/.test(n) ? 16 * parseFloat(n, 10) + "px" : /cm$/.test(n) ? 96 * parseFloat(n, 10) / 2.54 + "px" : "16px"), t = parseFloat(n, 10) * i
            } else t = 18;
            return t
        },
        _setBodyHeight: function() {
            if (!this.bStopCheckingBodyHeight) {
                var e, t, i = this.getDocument().body,
                    n = jindo.$Element(i),
                    o = parseInt(n.css("marginTop"), 10) + parseInt(n.css("marginBottom"), 10),
                    s = this.oApp.getEditingAreaHeight(),
                    r = s - o,
                    a = n.height();
                if (this.nTopBottomMargin = o, 0 === a) return n.css("height", r + "px"), void setTimeout(this.fnSetBodyHeight, 500);
                var l = jindo.$Agent().navigator(),
                    h = l.ie && 11 === l.nativeVersion,
                    d = this.nBodyHeight_last === a;
                h && d || n.css("height", "0px"), t = s < (e = parseInt(i.scrollHeight, 10)) ? e - o : r, this._isHorizontalScrollbarVisible() && (t -= this.nScrollbarWidth), h && d || n.css("height", t + "px"), this.nBodyHeight_last = t, setTimeout(this.fnSetBodyHeight, 500)
            }
        },
        _isHorizontalScrollbarVisible: function() {
            var e = this.getDocument();
            return e.documentElement.clientWidth < e.documentElement.scrollWidth
        },
        $ON_STOP_CHECKING_BODY_HEIGHT: function() {
            this.bStopCheckingBodyHeight || (this.bStopCheckingBodyHeight = !0)
        },
        $ON_START_CHECKING_BODY_HEIGHT: function() {
            this.bStopCheckingBodyHeight && (this.bStopCheckingBodyHeight = !1, this.fnSetBodyHeight())
        },
        $ON_IE_CHECK_EXCEPTION_FOR_SELECTION_PRESERVATION: function() {
            var e = this.getDocument().selection;
            e && "Control" === e.type && (this._oIERange = null)
        },
        _onIEBeforeDeactivate: function() {
            this.oApp.delayedExec("IE_CHECK_EXCEPTION_FOR_SELECTION_PRESERVATION", null, 0), this._oIERange || this._bIERangeReset || (this._oIERange = this.oApp.getSelection().cloneRange())
        },
        $ON_CHANGE_EDITING_MODE: function(e) {
            if (e === this.sMode) {
                var t = jindo.$Agent().navigator();
                if (t.ie && 8 < t.nativeVersion) {
                    var i = jindo.$$.getSingle("DIV.husky_seditor_editing_area_container").childNodes[0];
                    "DIV" == i.tagName && 1e3 == i.style.zIndex && i.parentNode.removeChild(i)
                }
                this.iframe.style.visibility = "visible", "block" != this.iframe.style.display && (this.iframe.style.display = "block"), this.oApp.exec("SET_EDITING_WINDOW", [this.getWindow()]), this.oApp.exec("START_CHECKING_BODY_HEIGHT")
            } else this.iframe.style.visibility = "hidden", this.oApp.exec("STOP_CHECKING_BODY_HEIGHT")
        },
        $AFTER_CHANGE_EDITING_MODE: function() {
            this._oIERange = null
        },
        $ON_ENABLE_WYSIWYG: function() {
            this._enableWYSIWYG()
        },
        $ON_DISABLE_WYSIWYG: function() {
            this._disableWYSIWYG()
        },
        $ON_IE_HIDE_CURSOR: function() {
            if (this._bIECursorHide) {
                this._onIEBeforeDeactivate();
                var e = this.oApp.getWYSIWYGDocument().selection;
                if (e && e.createRange) try {
                    e.empty()
                } catch (t) {
                    (e = this.oApp.getSelection()).select(), e.oBrowserSelection.selectNone()
                } else this.oApp.getEmptySelection().oBrowserSelection.selectNone(), this.getDocument().body.blur()
            }
        },
        $AFTER_SHOW_ACTIVE_LAYER: function() {
            this.oApp.exec("IE_HIDE_CURSOR"), this.bActiveLayerShown = !0
        },
        $BEFORE_EVENT_EDITING_AREA_KEYDOWN: function() {
            this._bKeyDown = !0
        },
        $ON_EVENT_EDITING_AREA_KEYDOWN: function(e) {
            if (this.oApp.getEditingMode() === this.sMode) {
                var t = e.key();
                if (this.oApp.oNavigator.ie) switch (t.keyCode) {
                    case 33:
                        this._pageUp(e);
                        break;
                    case 34:
                        this._pageDown(e);
                        break;
                    case 8:
                        this._backspace(e);
                        break;
                    case 46:
                        this._delete(e)
                } else this.oApp.oNavigator.firefox && 8 === t.keyCode && this._backspace(e);
                this._recordUndo(t)
            }
        },
        _backspace: function(e) {
            var t = this._prepareBackspaceDelete(!0);
            t && this._removeUnremovable(t, !0) && e.stop()
        },
        _delete: function(e) {
            var t = this._prepareBackspaceDelete(!1);
            if (t)
                if (this._removeUnremovable(t, !1)) e.stop();
                else if (3 === t.nodeType) {
                var i = this.oApp.getSelection().getLineInfo().oEnd.oLineBreaker,
                    n = i && i.nextSibling;
                this._removeWrongSpan(n)
            } else this._removeWrongSpan(t)
        },
        _prepareBackspaceDelete: function(e) {
            var t = this.oApp.getSelection();
            if (t.collapsed) {
                var i = t.getNodeAroundRange(e, !1);
                return this._isLineFeed(i) && (i = e ? t._getPrevNode(i) : t._getNextNode(i)), this._clearCursorHolderValue(i), i
            }
        },
        _isLineFeed: function(e) {
            return e && 3 === e.nodeType && /^[\n]*$/.test(e.nodeValue)
        },
        _clearCursorHolderValue: function(e) {
            !e || 3 !== e.nodeType || "​" !== e.nodeValue && "\ufeff" !== e.nodeValue || (e.nodeValue = "")
        },
        _removeUnremovable: function(e, t) {
            var i = !1;
            if (!e) return !1;
            if ("TABLE" === e.nodeName) e.parentNode.removeChild(e), i = !0;
            else if ("DIV" === e.nodeName) {
                var n = t ? e.lastChild : e.firstChild;
                n ? "TABLE" === n.nodeName ? (e.removeChild(n), i = !0) : 1 === n.nodeType && "" == jindo.$S(n.innerHTML).trim() && (e.removeChild(n), i = !0) : (e.parentNode.removeChild(e), i = !0)
            }
            return i
        },
        _removeWrongSpan: function(e) {
            if (e && "SPAN" === e.nodeName && e.firstChild && "P" === e.firstChild.nodeName) {
                for (var t = e.parentNode; e.firstChild;) t.insertBefore(e.firstChild, e);
                t.removeChild(e)
            }
        },
        $BEFORE_EVENT_EDITING_AREA_KEYUP: function() {
            if (!this._bKeyDown) return !1;
            this._bKeyDown = !1
        },
        $ON_EVENT_EDITING_AREA_MOUSEUP: function() {
            this.oApp.saveSnapShot()
        },
        $BEFORE_PASTE_HTML: function() {
            this.oApp.getEditingMode() !== this.sMode && this.oApp.exec("CHANGE_EDITING_MODE", [this.sMode])
        },
        $ON_PASTE_HTML: function(e, t, i) {
            var n, o, s, r, a, l, h;
            if (i = i || {}, this.oApp.getEditingMode() === this.sMode) {
                if (this.focus(), i.bNoUndo || this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["PASTE HTML"]), o = jindo.$Agent().navigator(), (n = t || this.oApp.getSelection()).pasteHTML(e, i.bBlock), o.ie ? (n.collapseToEnd(), n.select(), this._oIERange = null, this._bIERangeReset = !1) : (s = n.placeStringBookmark(), this.oApp.getWYSIWYGDocument().body.innerHTML = this.oApp.getWYSIWYGDocument().body.innerHTML, n.moveToBookmark(s), n.collapseToEnd(), n.select(), n.removeStringBookmark(s), n = this.oApp.getSelection(), t && t.setRange(n)), -1 < e.indexOf("<img") && 1 === (r = n.startContainer).nodeType && "P" === r.tagName && 0 < (a = jindo.$Element(r).child(function(e) {
                        return 1 === e.$value().nodeType && "IMG" === e.$value().tagName
                    }, 1)).length)
                    for (l = a[a.length - 1].$value().nextSibling; l;) h = l.nextSibling, 3 !== l.nodeType || "&nbsp;" !== l.nodeValue && l.nodeValue !== unescape("%u00A0") || r.removeChild(l), l = h;
                i.bNoUndo || this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["PASTE HTML"])
            }
        },
        $ON_FOCUS_N_CURSOR: function(e, t) {
            var i, n;
            if (t && (i = jindo.$(t, this.getDocument()))) return clearTimeout(this._nTimerFocus), void(this._nTimerFocus = setTimeout(jindo.$Fn(function(e) {
                this._scrollIntoView(e), this.oApp.exec("FOCUS")
            }, this).bind(i), 300));
            (n = this.oApp.getSelection()).collapsed ? e ? (this.oApp.exec("FOCUS"), i = this.getDocument().body, n.selectNode(i), n.collapseToEnd(), n.select(), this._scrollIntoView(i)) : this.oApp.exec("FOCUS") : (e ? n.collapseToEnd() : n.collapseToStart(), n.select())
        },
        _getElementVerticalPosition: function(e) {
            var t = 0,
                i = e,
                n = {
                    nTop: 0,
                    nBottom: 0
                };
            if (!e) return n;
            try {
                for (; i;) t += i.offsetTop, i = i.offsetParent
            } catch (o) {}
            return n.nTop = t, n.nBottom = t + jindo.$Element(e).height(), n
        },
        _getVisibleVerticalPosition: function() {
            var e, t, i, n = {
                nTop: 0,
                nBottom: 0
            };
            return e = this.getWindow(), t = this.getDocument(), i = e.innerHeight ? e.innerHeight : t.documentElement.clientHeight || t.body.clientHeight, n.nTop = e.pageYOffset || t.documentElement.scrollTop, n.nBottom = n.nTop + i, n
        },
        _isElementVisible: function(e, t) {
            return e.nTop >= t.nTop && e.nBottom <= t.nBottom
        },
        _scrollIntoView: function(e) {
            var t, i = this._getElementVerticalPosition(e),
                n = this._getVisibleVerticalPosition();
            this._isElementVisible(i, n) || (0 < (t = i.nBottom - n.nBottom) ? this.getWindow().scrollTo(0, n.nTop + t) : this.getWindow().scrollTo(0, i.nTop))
        },
        $BEFORE_MSG_EDITING_AREA_RESIZE_STARTED: function() {
            if (!jindo.$Agent().navigator().ie) {
                var e = null;
                e = this.oApp.getSelection(), this.sBM = e.placeStringBookmark()
            }
        },
        $AFTER_MSG_EDITING_AREA_RESIZE_ENDED: function() {
            if (this.oApp.getEditingMode() === this.sMode && !jindo.$Agent().navigator().ie) {
                var e = this.oApp.getEmptySelection();
                e.moveToBookmark(this.sBM), e.select(), e.removeStringBookmark(this.sBM)
            }
        },
        $ON_CLEAR_IE_BACKUP_SELECTION: function() {
            this._oIERange = null
        },
        $ON_RESTORE_IE_SELECTION: function() {
            if (this._oIERange) try {
                this._oIERange.select(), this._oPrevIERange = this._oIERange, this._oIERange = null
            } catch (e) {}
        },
        $ON_EVENT_EDITING_AREA_PASTE: function(e) {
            this.oApp.delayedExec("EVENT_EDITING_AREA_PASTE_DELAY", [e], 0)
        },
        $ON_EVENT_EDITING_AREA_PASTE_DELAY: function(e) {
            this._replaceBlankToNbsp(e.element)
        },
        _replaceBlankToNbsp: function(e) {
            var t = this.oApp.oNavigator;
            if (t.ie && 9 === t.nativeVersion && 7 === document.documentMode && 1 === e.nodeType && "BR" !== e.tagName) {
                var i = jindo.$$("p:empty()", this.oApp.getWYSIWYGDocument().body, {
                    oneTimeOffCache: !0
                });
                jindo.$A(i).forEach(function(e) {
                    e.innerHTML = "&nbsp;"
                })
            }
        },
        _pageUp: function(e) {
            var t, i = this._getEditorHeight(),
                n = jindo.$Document(this.oApp.getWYSIWYGDocument()).scrollPosition();
            t = n.top <= i ? 0 : n.top - i, this.oApp.getWYSIWYGWindow().scrollTo(0, t), e.stop()
        },
        _pageDown: function(e) {
            var t, i = this._getEditorHeight(),
                n = jindo.$Document(this.oApp.getWYSIWYGDocument()).scrollPosition(),
                o = this._getBodyHeight();
            t = n.top + i >= o ? o - i : n.top + i, this.oApp.getWYSIWYGWindow().scrollTo(0, t), e.stop()
        },
        _getEditorHeight: function() {
            return this.oApp.elEditingAreaContainer.offsetHeight - this.nTopBottomMargin
        },
        _getBodyHeight: function() {
            return parseInt(this.getDocument().body.scrollHeight, 10)
        },
        initIframe: function() {
            try {
                if (!this.iframe.contentWindow.document || !this.iframe.contentWindow.document.body || "about:blank" === this.iframe.contentWindow.document.location.href) throw new Error("Access denied");
                var e = nhn.husky.SE2M_Configuration.SE2M_CSSLoader && nhn.husky.SE2M_Configuration.SE2M_CSSLoader.sCSSBaseURI ? nhn.husky.SE2M_Configuration.SE2M_CSSLoader.sCSSBaseURI : "";
                if (nhn.husky.SE2M_Configuration.SE_EditingAreaManager.sCSSBaseURI && (e = nhn.husky.SE2M_Configuration.SE_EditingAreaManager.sCSSBaseURI), e) {
                    var t = e,
                        i = this.oApp && this.oApp.htOptions.I18N_LOCALE;
                    i && (t += "/" + i), t += "/smart_editor2_in.css";
                    var n = this.getDocument(),
                        o = n.getElementsByTagName("head")[0],
                        s = n.createElement("link");
                    s.type = "text/css", s.rel = "stylesheet", s.href = t, s.onload = jindo.$Fn(function() {
                        this.oApp && this.oApp.getEditingMode && this.oApp.getEditingMode() === this.sMode && this.oApp.exec("RESET_STYLE_STATUS"), s.onload = null
                    }, this).bind(), o.appendChild(s)
                }
                this._enableWYSIWYG(), this.status = nhn.husky.PLUGIN_STATUS.READY
            } catch (r) {
                if (!(0 < this._nIFrameReadyCount--)) throw "iframe for WYSIWYG editing mode can't be initialized. Please check if the iframe document exists and is also accessable(cross-domain issues). ";
                setTimeout(jindo.$Fn(this.initIframe, this).bind(), 100)
            }
        },
        getIR: function() {
            var e = this.iframe.contentWindow.document.body.innerHTML;
            return this.oApp.applyConverter ? this.oApp.applyConverter(this.sMode + "_TO_IR", e, this.oApp.getWYSIWYGDocument()) : e
        },
        setIR: function(e) {
            var t, i = this.oApp.oNavigator.ie && document.documentMode < 11,
                n = i ? "" : "<br>";
            if ("" === (t = this.oApp.applyConverter ? this.oApp.applyConverter("IR_TO_" + this.sMode, e, this.oApp.getWYSIWYGDocument()) : e).replace(/[\r\n\t\s]*/, "") && ("BR" !== this.oApp.sLineBreaker && (n = "<p>" + n + "</p>"), t = n), this.iframe.contentWindow.document.body.innerHTML = t, i && this.oApp.getEditingMode() === this.sMode)
                for (var o = this.oApp.getWYSIWYGDocument().body.getElementsByTagName("P"), s = 0, r = o.length; s < r; s++) 1 === o[s].childNodes.length && "&nbsp;" === o[s].innerHTML && (o[s].innerHTML = "")
        },
        getRawContents: function() {
            return this.iframe.contentWindow.document.body.innerHTML
        },
        getRawHTMLContents: function() {
            return this.getRawContents()
        },
        setRawHTMLContents: function(e) {
            this.iframe.contentWindow.document.body.innerHTML = e
        },
        getWindow: function() {
            return this.iframe.contentWindow
        },
        getDocument: function() {
            return this.iframe.contentWindow.document
        },
        focus: function() {
            this.getDocument().body.focus(), this.oApp.exec("RESTORE_IE_SELECTION")
        },
        _recordUndo: function(e) {
            33 <= e.keyCode && e.keyCode <= 40 ? this.oApp.saveSnapShot() : e.alt || e.ctrl || 16 === e.keyCode || this.oApp.getLastKey() !== e.keyCode && (this.oApp.setLastKey(e.keyCode), !e.enter && 46 !== e.keyCode && 8 !== e.keyCode || this.oApp.exec("RECORD_UNDO_ACTION", ["KEYPRESS(" + e.keyCode + ")", {
                bMustBlockContainer: !0
            }]))
        },
        _enableWYSIWYG: function() {
            null !== this.iframe.contentWindow.document.body.contentEditable ? this.iframe.contentWindow.document.body.contentEditable = !0 : this.iframe.contentWindow.document.designMode = "on", this.bWYSIWYGEnabled = !0, jindo.$Agent().navigator().firefox && setTimeout(jindo.$Fn(function() {
                this.iframe.contentWindow.document.execCommand("enableInlineTableEditing", !1, !1)
            }, this).bind(), 0)
        },
        _disableWYSIWYG: function() {
            null !== this.iframe.contentWindow.document.body.contentEditable ? this.iframe.contentWindow.document.body.contentEditable = !1 : this.iframe.contentWindow.document.designMode = "off", this.bWYSIWYGEnabled = !1
        },
        isWYSIWYGEnabled: function() {
            return this.bWYSIWYGEnabled
        }
    })
}, function(e, t) {
    nhn.husky.SE_EditingArea_HTMLSrc = jindo.$Class({
        name: "SE_EditingArea_HTMLSrc",
        sMode: "HTMLSrc",
        bAutoResize: !1,
        nMinHeight: null,
        $init: function(e) {
            this.elEditingArea = jindo.$(e)
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oNavigator = jindo.$Agent().navigator(), this.oApp.exec("REGISTER_EDITING_AREA", [this])
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.getEditingAreaHeight && (this.nMinHeight = this.oApp.getEditingAreaHeight())
        },
        $ON_CHANGE_EDITING_MODE: function(e) {
            e == this.sMode ? (this.elEditingArea.style.display = "block", this.elEditingArea.style.position = "absolute", this.elEditingArea.style.top = "0px") : (this.elEditingArea.style.display = "none", this.elEditingArea.style.position = "", this.elEditingArea.style.top = "")
        },
        $AFTER_CHANGE_EDITING_MODE: function(e, t) {
            e != this.sMode || t || new TextRange(this.elEditingArea).setSelection(0, 0)
        },
        startAutoResize: function() {
            var e = {
                nMinHeight: this.nMinHeight,
                wfnCallback: jindo.$Fn(this.oApp.checkResizeGripPosition, this).bind()
            };
            this.oNavigator.msafari && (e.wfnCallback = function() {}), this.bAutoResize = !0, this.AutoResizer = new nhn.husky.AutoResizer(this.elEditingArea, e), this.AutoResizer.bind()
        },
        stopAutoResize: function() {
            this.AutoResizer.unbind()
        },
        getIR: function() {
            var e = this.getRawContents();
            return this.oApp.applyConverter && (e = this.oApp.applyConverter(this.sMode + "_TO_IR", e, this.oApp.getWYSIWYGDocument())), e
        },
        setIR: function(e) {
            "<br>" !== e.toLowerCase() && "<p>&nbsp;</p>" !== e.toLowerCase() && "<p><br></p>" !== e.toLowerCase() && "<p></p>" !== e.toLowerCase() || (e = "");
            var t = jindo.$Agent().navigator();
            t.ie && 11 == t.nativeVersion && 11 == document.documentMode && (e = e.replace(/(<br><br>$)/, ""));
            var i = e;
            this.oApp.applyConverter && (i = this.oApp.applyConverter("IR_TO_" + this.sMode, i, this.oApp.getWYSIWYGDocument())), this.setRawContents(i)
        },
        setRawContents: function(e) {
            void 0 !== e && (this.elEditingArea.value = e)
        },
        getRawContents: function() {
            return this.elEditingArea.value
        },
        focus: function() {
            this.elEditingArea.focus()
        }
    }), "undefined" == typeof window.TextRange && (window.TextRange = {}), TextRange = function(e, t) {
        this._o = e, this._oDoc = t || document
    }, TextRange.prototype.getSelection = function() {
        var e = this._o,
            t = [-1, -1];
        if (isNaN(this._o.selectionStart)) {
            e.focus();
            var i = this._oDoc.body.createTextRange(),
                n = null;
            n = this._oDoc.selection.createRange().duplicate(), i.moveToElementText(e), n.collapse(!0), i.setEndPoint("EndToEnd", n), t[0] = i.text.length, n = this._oDoc.selection.createRange().duplicate(), i.moveToElementText(e), n.collapse(!1), i.setEndPoint("EndToEnd", n), t[1] = i.text.length, e.blur()
        } else t[0] = e.selectionStart, t[1] = e.selectionEnd;
        return t
    }, TextRange.prototype.setSelection = function(e, t) {
        var i = this._o;
        if (void 0 === t && (t = e), i.setSelectionRange) i.setSelectionRange(e, t);
        else if (i.createTextRange) {
            var n = i.createTextRange();
            n.collapse(!0), n.moveStart("character", e), n.moveEnd("character", t - e), n.select(), i.blur()
        }
    }, TextRange.prototype.copy = function() {
        var e = this.getSelection();
        return this._o.value.substring(e[0], e[1])
    }, TextRange.prototype.paste = function(e) {
        var t = this._o,
            i = this.getSelection(),
            n = t.value,
            o = n.substr(0, i[0]);
        n = o + e + n.substr(i[1]), t.value = n;
        var s = 0;
        if ("undefined" == typeof this._oDoc.body.style.maxHeight) {
            var r = o.match(/\n/gi);
            s = null !== r ? r.length : 0
        }
        this.setSelection(i[0] + e.length - s)
    }, TextRange.prototype.cut = function() {
        var e = this.copy();
        return this.paste(""), e
    }
}, function(e, t) {
    nhn.husky.SE_EditingArea_TEXT = jindo.$Class({
        name: "SE_EditingArea_TEXT",
        sMode: "TEXT",
        sRxConverter: "@[0-9]+@",
        bAutoResize: !1,
        nMinHeight: null,
        $init: function(e) {
            this.elEditingArea = jindo.$(e)
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oNavigator = jindo.$Agent().navigator(), this.oApp.exec("REGISTER_EDITING_AREA", [this]), this.oApp.exec("ADD_APP_PROPERTY", ["getTextAreaContents", jindo.$Fn(this.getRawContents, this).bind()])
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.getEditingAreaHeight && (this.nMinHeight = this.oApp.getEditingAreaHeight())
        },
        $ON_REGISTER_CONVERTERS: function() {
            this.oApp.exec("ADD_CONVERTER", ["IR_TO_TEXT", jindo.$Fn(this.irToText, this).bind()]), this.oApp.exec("ADD_CONVERTER", ["TEXT_TO_IR", jindo.$Fn(this.textToIr, this).bind()])
        },
        $ON_CHANGE_EDITING_MODE: function(e) {
            e == this.sMode ? (this.elEditingArea.style.display = "block", this.elEditingArea.style.position = "absolute", this.elEditingArea.style.top = "0px") : (this.elEditingArea.style.display = "none", this.elEditingArea.style.position = "", this.elEditingArea.style.top = "")
        },
        $AFTER_CHANGE_EDITING_MODE: function(e, t) {
            e != this.sMode || t || new TextRange(this.elEditingArea).setSelection(0, 0)
        },
        irToText: function(e) {
            var t, i = e,
                n = i.match(new RegExp(this.sRxConverter));
            return null !== n && (i = i.replace(new RegExp(this.sRxConverter), "")), -1 < (t = (i = (i = (i = (i = (i = (i = (i = (i = (i = (i = i.replace(/\r/g, "")).replace(/[\n|\t]/g, "")).replace(/[\v|\f]/g, "")).replace(/<p><br><\/p>/gi, "\n")).replace(/<P>&nbsp;<\/P>/gi, "\n")).replace(/<br(\s)*\/?>/gi, "\n")).replace(/<br(\s[^/]*)?>/gi, "\n")).replace(/<\/p(\s[^/]*)?>/gi, "\n")).replace(/<\/li(\s[^/]*)?>/gi, "\n")).replace(/<\/tr(\s[^/]*)?>/gi, "\n")).lastIndexOf("\n")) && "\n" == i.substring(t) && (i = i.substring(0, t)), i = jindo.$S(i).stripTags().toString(), i = this.unhtmlSpecialChars(i), null !== n && (i = n[0] + i), i
        },
        textToIr: function(e) {
            if (e) {
                var t, i = e;
                return null !== (t = i.match(new RegExp(this.sRxConverter))) && (i = i.replace(t[0], "")), i = this.htmlSpecialChars(i), i = this._addLineBreaker(i), null !== t && (i = t[0] + i), i
            }
        },
        _addLineBreaker: function(e) {
            if ("BR" === this.oApp.sLineBreaker) return e.replace(/\r?\n/g, "<BR>");
            for (var t = new StringBuffer, i = e.split("\n"), n = i.length, o = "", s = 0; s < n && (o = jindo.$S(i[s]).trim().$value(), s !== n - 1 || "" !== o); s++) null !== o && "" !== o ? (t.append("<P>"), t.append(i[s]), t.append("</P>")) : jindo.$Agent().navigator().ie ? t.append("<P>&nbsp;</P>") : t.append("<P><BR></P>");
            return t.toString()
        },
        startAutoResize: function() {
            var e = {
                nMinHeight: this.nMinHeight,
                wfnCallback: jindo.$Fn(this.oApp.checkResizeGripPosition, this).bind()
            };
            this.oNavigator.msafari && (e.wfnCallback = function() {}), this.bAutoResize = !0, this.AutoResizer = new nhn.husky.AutoResizer(this.elEditingArea, e), this.AutoResizer.bind()
        },
        stopAutoResize: function() {
            this.AutoResizer.unbind()
        },
        getIR: function() {
            var e = this.getRawContents();
            return this.oApp.applyConverter && (e = this.oApp.applyConverter(this.sMode + "_TO_IR", e, this.oApp.getWYSIWYGDocument())), e
        },
        setIR: function(e) {
            var t = e;
            this.oApp.applyConverter && (t = this.oApp.applyConverter("IR_TO_" + this.sMode, t, this.oApp.getWYSIWYGDocument())), this.setRawContents(t)
        },
        setRawContents: function(e) {
            void 0 !== e && (this.elEditingArea.value = e)
        },
        getRawContents: function() {
            return this.elEditingArea.value
        },
        focus: function() {
            this.elEditingArea.focus()
        },
        htmlSpecialChars: function(e) {
            return e.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/ /g, "&nbsp;")
        },
        unhtmlSpecialChars: function(e) {
            return e.replace(/&lt;/g, "<").replace(/&gt;/g, ">").replace(/&nbsp;/g, " ").replace(/&amp;/g, "&")
        }
    })
}, function(e, t) {
    nhn.husky.SE_EditingAreaVerticalResizer = jindo.$Class({
        name: "SE_EditingAreaVerticalResizer",
        oResizeGrip: null,
        sCookieNotice: "bHideResizeNotice",
        nEditingAreaMinHeight: null,
        htConversionMode: null,
        $init: function(e, t) {
            this.htConversionMode = t, this._assignHTMLElements(e)
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["isUseVerticalResizer", jindo.$Fn(this.isUseVerticalResizer, this).bind()])
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.bMobile ? (this.oResizeGrip.disabled = !0, this.oResizeGrip.style.height = "0") : (this.oApp.exec("REGISTER_HOTKEY", ["shift+esc", "FOCUS_RESIZER"]), this.isUseVerticalResizer() ? (this.oResizeGrip.style.display = "block", this.welNoticeLayer && !Number(jindo.$Cookie().get(this.sCookieNotice)) && (this.welNoticeLayer.delegate("click", "BUTTON.bt_clse", jindo.$Fn(this._closeNotice, this).bind()), this.welNoticeLayer.show()), this.$FnMouseDown = jindo.$Fn(this._mousedown, this), this.$FnMouseMove = jindo.$Fn(this._mousemove, this), this.$FnMouseUp = jindo.$Fn(this._mouseup, this), this.$FnMouseOver = jindo.$Fn(this._mouseover, this), this.$FnMouseOut = jindo.$Fn(this._mouseout, this), this.$FnMouseDown.attach(this.oResizeGrip, "mousedown"), this.$FnMouseOver.attach(this.oResizeGrip, "mouseover"), this.$FnMouseOut.attach(this.oResizeGrip, "mouseout")) : (this.oResizeGrip.style.display = "none", this.oApp.isUseModeChanger() || (this.elModeToolbar.style.display = "none"))), this.oApp.exec("ADD_APP_PROPERTY", ["checkResizeGripPosition", jindo.$Fn(this.checkResizeGripPosition, this).bind()]), this.oApp.getEditingAreaHeight && (this.nEditingAreaMinHeight = this.oApp.getEditingAreaHeight())
        },
        $ON_DISABLE_ALL_UI: function() {
            this.oResizeGrip.style.cursor = "default", this.welConversionMode.addClass("off"), this.oResizeGrip.disabled = !0
        },
        $ON_ENABLE_ALL_UI: function() {
            this.oResizeGrip.style.cursor = "n-resize", this.welConversionMode.removeClass("off"), this.oResizeGrip.disabled = !1
        },
        isUseVerticalResizer: function() {
            return "undefined" == typeof this.htConversionMode || "undefined" == typeof this.htConversionMode.bUseVerticalResizer || !0 === this.htConversionMode.bUseVerticalResizer
        },
        checkResizeGripPosition: function(e) {
            var t = jindo.$Document(),
                i = jindo.$Element(this.oResizeGrip).offset().top - t.scrollPosition().top + 25 - t.clientSize().height;
            i <= 0 || (e && (this.nEditingAreaMinHeight > this.oApp.getEditingAreaHeight() - i && (i = -1 * (this.nEditingAreaMinHeight - this.oApp.getEditingAreaHeight())), this.oApp.exec("MSG_EDITING_AREA_RESIZE_STARTED"), this.oApp.exec("RESIZE_EDITING_AREA_BY", [0, -1 * i]), this.oApp.exec("MSG_EDITING_AREA_RESIZE_ENDED")), this.oApp.exec("STOP_AUTORESIZE_EDITING_AREA"))
        },
        $ON_FOCUS_RESIZER: function() {
            this.oApp.exec("IE_HIDE_CURSOR"), this.oResizeGrip.focus()
        },
        _assignHTMLElements: function(e) {
            this.oResizeGrip = jindo.$$.getSingle("BUTTON.husky_seditor_editingArea_verticalResizer", e), this.elModeToolbar = jindo.$$.getSingle("DIV.se2_conversion_mode", e), this.welNoticeLayer = jindo.$Element(jindo.$$.getSingle("DIV.husky_seditor_resize_notice", e)), this.welConversionMode = jindo.$Element(this.oResizeGrip.parentNode)
        },
        _mouseover: function(e) {
            e.stopBubble(), this.welConversionMode.addClass("controller_on")
        },
        _mouseout: function(e) {
            e.stopBubble(), this.welConversionMode.removeClass("controller_on")
        },
        _mousedown: function(e) {
            this.iStartHeight = e.pos().clientY, this.iStartHeightOffset = e.pos().layerY, this.$FnMouseMove.attach(document, "mousemove"), this.$FnMouseUp.attach(document, "mouseup"), this.iStartHeight = e.pos().clientY, this.oApp.exec("HIDE_ACTIVE_LAYER"), this.oApp.exec("HIDE_ALL_DIALOG_LAYER"), this.oApp.exec("MSG_EDITING_AREA_RESIZE_STARTED", [this.$FnMouseDown, this.$FnMouseMove, this.$FnMouseUp])
        },
        _mousemove: function(e) {
            var t = e.pos().clientY - this.iStartHeight;
            this.oApp.exec("RESIZE_EDITING_AREA_BY", [0, t])
        },
        _mouseup: function() {
            this.$FnMouseMove.detach(document, "mousemove"), this.$FnMouseUp.detach(document, "mouseup"), this.oApp.exec("MSG_EDITING_AREA_RESIZE_ENDED", [this.$FnMouseDown, this.$FnMouseMove, this.$FnMouseUp])
        },
        _closeNotice: function() {
            this.welNoticeLayer.hide(), jindo.$Cookie().set(this.sCookieNotice, 1, 3650)
        }
    })
}, function(e, t) {
    nhn.husky.SE_WYSIWYGEnterKey = jindo.$Class({
        name: "SE_WYSIWYGEnterKey",
        $init: function(e) {
            this.sLineBreaker = "BR" == e ? "BR" : "P", this.htBrowser = jindo.$Agent().navigator(), this.htBrowser.opera && "P" == this.sLineBreaker && (this.$ON_MSG_APP_READY = function() {}), this.htBrowser.ie ? this._addCursorHolder = this._addCursorHolderSpace : (this._addExtraCursorHolder = function() {}, this._addBlankText = function() {})
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["sLineBreaker", this.sLineBreaker]), this.oSelection = this.oApp.getEmptySelection(), this.tmpTextNode = this.oSelection._document.createTextNode(unescape("%u00A0")), jindo.$Fn(this._onKeyDown, this).attach(this.oApp.getWYSIWYGDocument(), "keydown")
        },
        _onKeyDown: function(e) {
            var t = e.key();
            t.shift || t.enter && ("BR" == this.sLineBreaker ? this._insertBR(e) : this._wrapBlock(e))
        },
        $ON_REGISTER_CONVERTERS: function() {
            this.oApp.exec("ADD_CONVERTER", ["IR_TO_DB", jindo.$Fn(this.onIrToDB, this).bind()])
        },
        onIrToDB: function(e) {
            var t = e,
                i = /(<p[^>]*>)(?:\s*)(<\/p>)/gi;
            return t = this.htBrowser.ie && this.htBrowser.version < 11 ? t.replace(i, "$1&nbsp;$2") : t.replace(i, "")
        },
        _addBlankText: function(e) {
            var t, i, n, o, s, r = e.getNodes();
            for (t = 0, i = r.length; t < i; t++) 1 === (n = r[t]).nodeType && "SPAN" === n.tagName && (-1 < n.id.indexOf(e.HUSKY_BOOMARK_START_ID_PREFIX) || -1 < n.id.indexOf(e.HUSKY_BOOMARK_END_ID_PREFIX) || (!(o = n.firstChild) || 3 == o.nodeType && nhn.husky.SE2M_Utils.isBlankTextNode(o) || 1 == o.nodeType && 1 == n.childNodes.length && (-1 < o.id.indexOf(e.HUSKY_BOOMARK_START_ID_PREFIX) || -1 < o.id.indexOf(e.HUSKY_BOOMARK_END_ID_PREFIX))) && (s = e._document.createTextNode(unescape("%uFEFF")), n.appendChild(s)))
        },
        _addCursorHolder: function(e) {
            var t = e;
            return "" != e.innerHTML && !(t = this._getStyleOnlyNode(e)) || (t.innerHTML = "<br>"), t = t || this._getStyleNode(e)
        },
        _addCursorHolderSpace: function(e) {
            var t;
            if (this._addSpace(e), "" == (t = this._getStyleNode(e)).innerHTML && "param" != t.nodeName.toLowerCase()) try {
                t.innerHTML = unescape("%uFEFF")
            } catch (i) {}
            return t
        },
        _getBlockEndNode: function(e, t) {
            return e ? "BR" === e.nodeName ? e : e === t ? t : this._getBlockEndNode(e.nextSibling, t) : t
        },
        _convertHeadSpace: function(e) {
            if (e && 3 === e.nodeType) {
                for (var t, i = e.nodeValue, n = "", o = 0;
                    (t = i[o]) && " " === t; o++) n += " ";
                0 < o && (e.nodeValue = n + i.substring(o))
            }
        },
        _getValidNextSibling: function(e) {
            var t = e.nextSibling;
            return t ? 3 == t.nodeType && "" == t.nodeValue ? arguments.callee(t) : t : null
        },
        _wrapBlock: function(e) {
            var t, i, n, o = this.oApp.getSelection(),
                s = o.placeStringBookmark(),
                r = o.getLineInfo(),
                a = r.oStart,
                l = r.oEnd;
            if ((!a.bParentBreak || o.rxBlockContainer.test(a.oLineBreaker.tagName)) && (e.stop(), o.deleteContents(), a.oNode.parentNode && 11 !== a.oNode.parentNode.nodeType)) {
                t = this.oApp.getWYSIWYGDocument().createElement(this.sLineBreaker), o.moveToBookmark(s), o.setStartBefore(a.oNode), o.surroundContents(t), o.collapseToEnd(), i = this.oApp.getWYSIWYGDocument().createElement(this.sLineBreaker);
                var h = this._getBlockEndNode(a.oNode, l.oNode);
                return h === a.oNode && (h = l.oNode), o.setEndAfter(h), this._addBlankText(o), o.surroundContents(i), o.moveToStringBookmark(s, !0), o.collapseToEnd(), o.removeStringBookmark(s), o.select(), this._addCursorHolder(t), null !== i.lastChild && "BR" == i.lastChild.tagName && i.removeChild(i.lastChild), n = this._addCursorHolder(i), i.nextSibling && "BR" == i.nextSibling.tagName && i.parentNode.removeChild(i.nextSibling), o.selectNodeContents(n), o.collapseToStart(), o.select(), this.oApp.exec("CHECK_STYLE_CHANGE"), s = o.placeStringBookmark(), void setTimeout(jindo.$Fn(function(e) {
                    o.getStringBookmark(e) && (o.moveToStringBookmark(e), o.select(), o.removeStringBookmark(e))
                }, this).bind(s), 0)
            }
            var d = o.getStringBookmark(s, !0);
            if (this.htBrowser.firefox) d && d.nextSibling && "IFRAME" == d.nextSibling.tagName ? setTimeout(jindo.$Fn(function(e) {
                o.getStringBookmark(e) && (o.moveToStringBookmark(e), o.select(), o.removeStringBookmark(e))
            }, this).bind(s), 0) : (setTimeout(jindo.$Fn(function(e) {
                this._convertHeadSpace(e)
            }, this).bind(d.nextSibling), 0), o.removeStringBookmark(s));
            else if (this.htBrowser.ie) {
                var c, p, u = d.parentNode;
                if (this._firstBR2Line(u, o, l.oLineBreaker), !d || !u) return void o.removeStringBookmark(s);
                if (setTimeout(jindo.$Fn(function() {
                        this.oApp.getSelection().removeStringBookmark(s)
                    }, this).bind(s), 0), c = "U" === u.tagName || null !== nhn.husky.SE2M_Utils.findAncestorByTagName("U", u), p = "S" === u.tagName || "STRIKE" === u.tagName || null !== nhn.husky.SE2M_Utils.findAncestorByTagName("S", u) && null !== nhn.husky.SE2M_Utils.findAncestorByTagName("STRIKE", u), c || p) return void setTimeout(jindo.$Fn(this._addTextDecorationTag, this).bind(c, p), 0);
                setTimeout(jindo.$Fn(this._addExtraCursorHolder, this).bind(u), 0)
            } else {
                u = d.parentNode;
                var _ = this._getValidNextSibling(d);
                if ("SPAN" == u.tagName && _ && "BR" == _.nodeName && _.nextSibling) {
                    o.deleteContents(), o.setEndNodes(d, l.oLineBreaker);
                    var E = o.extractContents(),
                        g = E.firstChild;
                    u = a.oLineBreaker.parentNode, (_ = a.oLineBreaker.nextSibling) ? u.insertBefore(E, _) : u.appendChild(E), o.selectNodeContents(g), o.collapseToStart(), o.select(), o.removeStringBookmark(s), e.stop()
                } else this._convertHeadSpace(d.nextSibling), o.removeStringBookmark(s)
            }
        },
        _firstBR2Line: function(e, t, i) {
            for (; e.firstChild && "BR" === e.firstChild.nodeName;) {
                var n = t._document.createTextNode("​");
                e.replaceChild(n, e.firstChild), t.setStartBefore(i), t.setEndAfter(n), i.parentNode.insertBefore(t.extractContents(), i)
            }
        },
        _addExtraCursorHolder: function(e) {
            var t, i;
            if ((e = this._getStyleOnlyNode(e)) && "SPAN" === e.tagName) {
                for (t = e.lastChild; t;) i = t.previousSibling, t = (3 === t.nodeType && nhn.husky.SE2M_Utils.isBlankTextNode(t) && t.parentNode.removeChild(t), i);
                "" === e.innerHTML.replace("​", "").replace("\ufeff", "") && (e.innerHTML = "​")
            }
            var n, o = this.oApp.getSelection();
            o.collapsed && (o.fixCommonAncestorContainer(), (n = o.commonAncestorContainer) && (3 === (n = o._getVeryFirstRealChild(n)).nodeType && (n = n.parentNode), n && "SPAN" === n.tagName && ("" === n.innerHTML.replace("​", "").replace("\ufeff", "") && (n.innerHTML = "​"), o.selectNodeContents(n), o.collapseToStart(), o.select())))
        },
        _addSpace: function(e) {
            var t, i, n, o, s;
            if (e) {
                if (3 === e.nodeType) return e.parentNode;
                if ("P" !== e.tagName) return e;
                if (0 < (s = jindo.$Element(e).child(function(e) {
                        return 1 === e.$value().nodeType && "IMG" === e.$value().tagName
                    }, 1)).length) {
                    for (i = s[s.length - 1].$value().nextSibling; i;) n = i.nextSibling, 3 !== i.nodeType || "&nbsp;" !== i.nodeValue && i.nodeValue !== unescape("%u00A0") && "​" !== i.nodeValue || e.removeChild(i), i = n;
                    return e
                }
                for (n = i = e.firstChild, o = !1; i;) n = i.nextSibling, 3 === i.nodeType && (i.nodeValue === unescape("%uFEFF") && e.removeChild(i), o || "&nbsp;" !== i.nodeValue && i.nodeValue !== unescape("%u00A0") && "​" !== i.nodeValue || (o = !0)), i = n;
                return o || (t = this.tmpTextNode.cloneNode(), e.appendChild(t)), e
            }
        },
        _addTextDecorationTag: function(e, t) {
            var i, n, o = this.oApp.getSelection();
            if (o.collapsed) {
                for (i = o.startContainer; i;) {
                    if (3 === i.nodeType) {
                        i = nhn.DOMFix.parentNode(i);
                        break
                    }
                    if (!i.childNodes || 0 === i.childNodes.length) break;
                    i = i.firstChild
                }
                i && "U" !== i.tagName && "S" !== i.tagName && "STRIKE" !== i.tagName && (e && (n = o._document.createElement("U"), i.appendChild(n), i = n), t && (n = o._document.createElement("STRIKE"), i.appendChild(n)), n.innerHTML = "​", o.selectNodeContents(n), o.collapseToEnd(), o.select())
            }
        },
        _getStyleNode: function(e) {
            for (; e.firstChild && this.oSelection._isBlankTextNode(e.firstChild);) e.removeChild(e.firstChild);
            var t = e.firstChild;
            return t ? 3 === t.nodeType || 1 === t.nodeType && ("IMG" == t.tagName || "BR" == t.tagName || "HR" == t.tagName || "IFRAME" == t.tagName) ? e : this._getStyleNode(e.firstChild) : e
        },
        _getStyleOnlyNode: function(e) {
            if (!e) return null;
            if (!e.insertBefore) return null;
            if ("IMG" == e.tagName || "BR" == e.tagName || "HR" == e.tagName || "IFRAME" == e.tagName) return null;
            for (; e.firstChild && this.oSelection._isBlankTextNode(e.firstChild);) e.removeChild(e.firstChild);
            return 1 < e.childNodes.length ? null : e.firstChild ? 3 === e.firstChild.nodeType ? nhn.husky.SE2M_Utils.isBlankTextNode(e.firstChild) ? e : null : this._getStyleOnlyNode(e.firstChild) : e
        },
        _insertBR: function(e) {
            e.stop();
            var t = this.oApp.getSelection(),
                i = this.oApp.getWYSIWYGDocument().createElement("BR");
            if (t.insertNode(i), t.selectNode(i), t.collapseToEnd(), !this.htBrowser.ie) {
                var n = t.getLineInfo().oEnd;
                if (n.bParentBreak) {
                    for (; n.oNode && 3 == n.oNode.nodeType && "" == n.oNode.nodeValue;) n.oNode = n.oNode.previousSibling;
                    var o = 1;
                    n.oNode != i && n.oNode.nextSibling != i || (o = 0), 0 === o && (t.pasteHTML("<br type='_moz'/>"), t.collapseToEnd())
                }
            }
            t.insertNode(this.oApp.getWYSIWYGDocument().createTextNode("")), t.select()
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_EditingModeChanger = jindo.$Class({
        name: "SE2M_EditingModeChanger",
        htConversionMode: null,
        $init: function(e, t) {
            this.htConversionMode = t, this._assignHTMLElements(e)
        },
        _assignHTMLElements: function(e) {
            e = jindo.$(e) || document, this.elWYSIWYGButton = jindo.$$.getSingle("BUTTON.se2_to_editor", e), this.elHTMLSrcButton = jindo.$$.getSingle("BUTTON.se2_to_html", e), this.elTEXTButton = jindo.$$.getSingle("BUTTON.se2_to_text", e), this.elModeToolbar = jindo.$$.getSingle("DIV.se2_conversion_mode", e), this.welWYSIWYGButtonLi = jindo.$Element(this.elWYSIWYGButton.parentNode), this.welHTMLSrcButtonLi = jindo.$Element(this.elHTMLSrcButton.parentNode), this.welTEXTButtonLi = jindo.$Element(this.elTEXTButton.parentNode)
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["isUseModeChanger", jindo.$Fn(this.isUseModeChanger, this).bind()])
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.htOptions.bOnlyTextMode ? (this.elWYSIWYGButton.style.display = "none", this.elHTMLSrcButton.style.display = "none", this.elTEXTButton.style.display = "block", this.oApp.exec("CHANGE_EDITING_MODE", ["TEXT"])) : (this.oApp.registerBrowserEvent(this.elWYSIWYGButton, "click", "EVENT_CHANGE_EDITING_MODE_CLICKED", ["WYSIWYG"]), this.oApp.registerBrowserEvent(this.elHTMLSrcButton, "click", "EVENT_CHANGE_EDITING_MODE_CLICKED", ["HTMLSrc"]), this.oApp.registerBrowserEvent(this.elTEXTButton, "click", "EVENT_CHANGE_EDITING_MODE_CLICKED", ["TEXT", !1]), this.showModeChanger(), !1 === this.isUseModeChanger() && !1 === this.oApp.isUseVerticalResizer() && (this.elModeToolbar.style.display = "none"))
        },
        showModeChanger: function() {
            this.isUseModeChanger() ? (this.elWYSIWYGButton.style.display = "block", this.elHTMLSrcButton.style.display = "block", this.elTEXTButton.style.display = "block") : (this.elWYSIWYGButton.style.display = "none", this.elHTMLSrcButton.style.display = "none", this.elTEXTButton.style.display = "none")
        },
        isUseModeChanger: function() {
            return "undefined" == typeof this.htConversionMode || "undefined" == typeof this.htConversionMode.bUseModeChanger || !0 === this.htConversionMode.bUseModeChanger
        },
        $ON_EVENT_CHANGE_EDITING_MODE_CLICKED: function(e, t) {
            if ("TEXT" == e) {
                if (0 < this.oApp.getIR().length && !t && !confirm(this.oApp.$MSG("SE2M_EditingModeChanger.confirmTextMode"))) return !1;
                this.oApp.exec("CHANGE_EDITING_MODE", [e])
            } else this.oApp.exec("CHANGE_EDITING_MODE", [e]);
            "HTMLSrc" == e ? this.oApp.exec("MSG_NOTIFY_CLICKCR", ["htmlmode"]) : "TEXT" == e ? this.oApp.exec("MSG_NOTIFY_CLICKCR", ["textmode"]) : this.oApp.exec("MSG_NOTIFY_CLICKCR", ["editormode"])
        },
        $ON_DISABLE_ALL_UI: function(e) {
            e = e || {}, jindo.$A(e.aExceptions || []).has("mode_switcher") || ("WYSIWYG" == this.oApp.getEditingMode() ? (this.welWYSIWYGButtonLi.removeClass("active"), this.elHTMLSrcButton.disabled = !0, this.elTEXTButton.disabled = !0) : "TEXT" == this.oApp.getEditingMode() ? (this.welTEXTButtonLi.removeClass("active"), this.elWYSIWYGButton.disabled = !0, this.elHTMLSrcButton.disabled = !0) : (this.welHTMLSrcButtonLi.removeClass("active"), this.elWYSIWYGButton.disabled = !0, this.elTEXTButton.disabled = !0))
        },
        $ON_ENABLE_ALL_UI: function() {
            "WYSIWYG" == this.oApp.getEditingMode() ? (this.welWYSIWYGButtonLi.addClass("active"), this.elHTMLSrcButton.disabled = !1, this.elTEXTButton.disabled = !1) : "TEXT" == this.oApp.getEditingMode() ? (this.welTEXTButtonLi.addClass("active"), this.elWYSIWYGButton.disabled = !1, this.elHTMLSrcButton.disabled = !1) : (this.welHTMLSrcButtonLi.addClass("active"), this.elWYSIWYGButton.disabled = !1, this.elTEXTButton.disabled = !1)
        },
        $ON_CHANGE_EDITING_MODE: function(e) {
            "HTMLSrc" == e ? (this.welWYSIWYGButtonLi.removeClass("active"), this.welHTMLSrcButtonLi.addClass("active"), this.welTEXTButtonLi.removeClass("active"), this.elWYSIWYGButton.disabled = !1, this.elHTMLSrcButton.disabled = !0, this.elTEXTButton.disabled = !1, this.oApp.exec("HIDE_ALL_DIALOG_LAYER"), this.oApp.exec("DISABLE_ALL_UI", [{
                aExceptions: ["mode_switcher"]
            }])) : "TEXT" == e ? (this.welWYSIWYGButtonLi.removeClass("active"), this.welHTMLSrcButtonLi.removeClass("active"), this.welTEXTButtonLi.addClass("active"), this.elWYSIWYGButton.disabled = !1, this.elHTMLSrcButton.disabled = !1, this.elTEXTButton.disabled = !0, this.oApp.exec("HIDE_ALL_DIALOG_LAYER"), this.oApp.exec("DISABLE_ALL_UI", [{
                aExceptions: ["mode_switcher"]
            }])) : (this.welWYSIWYGButtonLi.addClass("active"), this.welHTMLSrcButtonLi.removeClass("active"), this.welTEXTButtonLi.removeClass("active"), this.elWYSIWYGButton.disabled = !0, this.elHTMLSrcButton.disabled = !1, this.elTEXTButton.disabled = !1, this.oApp.exec("RESET_STYLE_STATUS"), this.oApp.exec("ENABLE_ALL_UI", []))
        }
    })
}, function(e, t) {
    nhn.husky.SE_PasteHandler = jindo.$Class({
        name: "SE_PasteHandler",
        _rxStyleTag: /<style(?:\s+[^>]*)?>(?:.|\r|\n)*?<\/style>/gi,
        _rxStyleTagStrip: /<\/?style(?:\s+[^>]*)?>/gi,
        _rxClassSelector: /\w*\.\w+/g,
        _rxClassSelectorStart: /\w*\./g,
        $init: function() {},
        _getTmpDocument: function() {
            if (!this._oTmpDoc) {
                var e = document.createElement("IFRAME");
                e.style.display = "none", document.body.appendChild(e);
                var t = e.contentWindow.document;
                t.open(), t.write("<html><head></head><body></body></html>"), t.close(), this._oTmpDoc = t
            }
            return this._oTmpDoc
        },
        _getCSSStyleRule: function(e) {
            for (var t, i = {}, n = 0; t = e[n]; n++) t.type === CSSRule.STYLE_RULE && (i[t.selectorText] = t.style);
            return i
        },
        _extractStyle: function(e) {
            if (e) {
                var t = (e.match(this._rxStyleTag) || []).join("\n");
                t = t.replace(this._rxStyleTagStrip, "");
                var i = this._getTmpDocument(),
                    n = i.getElementsByTagName("HEAD")[0],
                    o = i.createElement("STYLE");
                o.innerHTML = t, n.appendChild(o), this._htStyleRule = this._getCSSStyleRule(o.sheet.cssRules), o.parentNode.removeChild(o)
            }
        },
        _applyInlineStyle: function(e, t, i) {
            for (var n, o = 0, s = t.length; o < s; o++) n = t[o], !i && e.style[n] || (e.style[n] = t[n])
        },
        _applyStyleRuleToInline: function(e, t, i) {
            var n = e.match(this._rxClassSelector) || [];
            if (!(n.length < 1))
                for (var o, s = n.join(" ").replace(this._rxClassSelectorStart, ""), r = jindo.$$(e, i), a = 0; o = r[a]; a++) this._applyInlineStyle(o, t), s && jindo.$Element(o).removeClass(s)
        },
        _applyStyle: function(e) {
            var t = this._htStyleRule || {};
            for (var i in t) this._applyStyleRuleToInline(i, t[i], e);
            this._htStyleRule = null
        },
        _revertFontAfterPaste: function(e) {
            nhn.husky.SE2M_Utils.removeInvalidFont(e), document.documentMode < 11 ? nhn.husky.SE2M_Utils.stripTags(e, "FONT") : nhn.husky.SE2M_Utils.convertFontToSpan(e)
        },
        _removeLineHeightInSpan: function(e) {
            for (var t, i = jindo.$$('span[style*="line-height:"]', e), n = 0; t = i[n]; n++) t.style.lineHeight = null
        },
        $ON_EVENT_EDITING_AREA_PASTE: function(e) {
            var t = e.$value().clipboardData;
            if (t) {
                var i = t.getData("text/html");
                i && this._extractStyle(i)
            }
        },
        $ON_EVENT_EDITING_AREA_PASTE_DELAY: function() {
            var e = this.oApp.getWYSIWYGDocument().body;
            this._revertFontAfterPaste(e), this._applyStyle(e), this._removeLineHeightInSpan(e)
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_ExecCommand = jindo.$Class({
        name: "SE2M_ExecCommand",
        oEditingArea: null,
        oUndoOption: null,
        _rxCmdInline: /^(?:bold|underline|italic|strikethrough|superscript|subscript)$/i,
        $init: function(e) {
            this.oEditingArea = e, this.nIndentSpacing = 40, this.rxClickCr = new RegExp("^bold|underline|italic|strikethrough|justifyleft|justifycenter|justifyright|justifyfull|insertorderedlist|insertunorderedlist|outdent|indent$", "i")
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oEditingArea && "IFRAME" == this.oEditingArea.tagName && (this.oEditingArea = this.oEditingArea.contentWindow.document)
        },
        $ON_MSG_APP_READY: function() {
            jindo.$Agent().os().mac ? (this.oApp.exec("REGISTER_HOTKEY", ["meta+b", "EXECCOMMAND", ["bold", !1, !1]]), this.oApp.exec("REGISTER_HOTKEY", ["meta+u", "EXECCOMMAND", ["underline", !1, !1]]), this.oApp.exec("REGISTER_HOTKEY", ["meta+i", "EXECCOMMAND", ["italic", !1, !1]]), this.oApp.exec("REGISTER_HOTKEY", ["meta+d", "EXECCOMMAND", ["strikethrough", !1, !1]])) : (this.oApp.exec("REGISTER_HOTKEY", ["ctrl+b", "EXECCOMMAND", ["bold", !1, !1]]), this.oApp.exec("REGISTER_HOTKEY", ["ctrl+u", "EXECCOMMAND", ["underline", !1, !1]]), this.oApp.exec("REGISTER_HOTKEY", ["ctrl+i", "EXECCOMMAND", ["italic", !1, !1]]), this.oApp.exec("REGISTER_HOTKEY", ["ctrl+d", "EXECCOMMAND", ["strikethrough", !1, !1]])), this.oApp.exec("REGISTER_HOTKEY", ["tab", "INDENT"]), this.oApp.exec("REGISTER_HOTKEY", ["shift+tab", "OUTDENT"]), this.oApp.exec("REGISTER_UI_EVENT", ["bold", "click", "EXECCOMMAND", ["bold", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["underline", "click", "EXECCOMMAND", ["underline", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["italic", "click", "EXECCOMMAND", ["italic", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["lineThrough", "click", "EXECCOMMAND", ["strikethrough", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["superscript", "click", "EXECCOMMAND", ["superscript", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["subscript", "click", "EXECCOMMAND", ["subscript", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["justifyleft", "click", "EXECCOMMAND", ["justifyleft", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["justifycenter", "click", "EXECCOMMAND", ["justifycenter", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["justifyright", "click", "EXECCOMMAND", ["justifyright", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["justifyfull", "click", "EXECCOMMAND", ["justifyfull", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["orderedlist", "click", "EXECCOMMAND", ["insertorderedlist", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["unorderedlist", "click", "EXECCOMMAND", ["insertunorderedlist", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["outdent", "click", "EXECCOMMAND", ["outdent", !1, !1]]), this.oApp.exec("REGISTER_UI_EVENT", ["indent", "click", "EXECCOMMAND", ["indent", !1, !1]]), this.oNavigator = jindo.$Agent().navigator(), this.oNavigator.safari || this.oNavigator.chrome || (this._getDocumentBR = function() {}, this._fixDocumentBR = function() {}), this.oNavigator.ie || (this._fixCorruptedBlockQuote = function() {}, this.oNavigator.safari || this.oNavigator.chrome || (this._insertBlankLine = function() {})), this.oNavigator.firefox || (this._extendBlock = function() {})
        },
        $ON_INDENT: function() {
            this.oApp.delayedExec("EXECCOMMAND", ["indent", !1, !1], 0)
        },
        $ON_OUTDENT: function() {
            this.oApp.delayedExec("EXECCOMMAND", ["outdent", !1, !1], 0)
        },
        $BEFORE_EXECCOMMAND: function(e, t, i, n) {
            this.oApp.exec("FOCUS"), this._bOnlyCursorChanged = !1;
            for (var o, s = 0, r = this.oApp.getSelection().getNodes(); o = r[s]; s++) nhn.husky.SE2M_Utils.removeInvalidNodeInTable(o);
            /^insertorderedlist|insertunorderedlist$/i.test(e) && (this._getDocumentBR(), this._checkBlockQuoteCondition_IE()), /^justify*/i.test(e) && this._removeElementAlign("span"), this._rxCmdInline.test(e) && (this.oUndoOption = {
                bMustBlockElement: !0
            }, nhn.CurrentSelection.isCollapsed() && (this._bOnlyCursorChanged = !0)), "indent" != e && "outdent" != e || ((n = n || {}).bDontAddUndoHistory = !0), n && n.bDontAddUndoHistory || this._bOnlyCursorChanged || (/^justify*/i.test(e) ? this.oUndoOption = {
                sSaveTarget: "BODY"
            } : "insertorderedlist" !== e && "insertunorderedlist" !== e || (this.oUndoOption = {
                bMustBlockContainer: !0
            }), this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", [e, this.oUndoOption])), this.oNavigator.ie && this.oApp.getWYSIWYGDocument().selection && "Control" === this.oApp.getWYSIWYGDocument().selection.type && this.oApp.getSelection().select(), "insertorderedlist" != e && "insertunorderedlist" != e || this._insertBlankLine()
        },
        _checkBlockQuoteCondition_IE: function() {
            var e, t = jindo.$Agent().navigator(),
                i = !1;
            if (t.ie && 9 <= t.nativeVersion && t.nativeVersion <= 11 && 9 <= t.version && t.version <= 11 || this.oApp.oAgent.os().winxp && t.ie && t.nativeVersion <= 8 || t.edge) {
                var n = this.oApp.getSelection().commonAncestorContainer,
                    o = nhn.husky.SE2M_Utils.findAncestorByTagNameWithCount("BLOCKQUOTE", n);
                if (e = o.elNode) {
                    var s = nhn.husky.SE2M_Utils.findClosestAncestorAmongTagNamesWithCount(["td", "th"], n);
                    s.elNode ? s.nRecursiveCount > o.nRecursiveCount && (i = !0) : i = !0
                }
            }
            i && this._insertDummyParagraph_IE(e)
        },
        _insertDummyParagraph_IE: function(e) {
            this._elDummyParagraph = document.createElement("P"), e.appendChild(this._elDummyParagraph)
        },
        _removeDummyParagraph_IE: function() {
            this._elDummyParagraph && this._elDummyParagraph.parentNode && this._elDummyParagraph.parentNode.removeChild(this._elDummyParagraph)
        },
        $ON_EXECCOMMAND: function(e, t, i) {
            var n = {},
                o = this.oApp.getSelection();
            if (t = !("" != t && !t) && t, i = !("" != i && !i) && i, this.oApp.exec("IS_SELECTED_TD_BLOCK", ["bIsSelectedTd", n]), n.bIsSelectedTd) "indent" == e ? this.oApp.exec("SET_LINE_BLOCK_STYLE", [null, jindo.$Fn(this._indentMargin, this).bind()]) : "outdent" == e ? this.oApp.exec("SET_LINE_BLOCK_STYLE", [null, jindo.$Fn(this._outdentMargin, this).bind()]) : this._setBlockExecCommand(e, t, i);
            else switch (e) {
                case "indent":
                case "outdent":
                    this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", [e]);
                    var s = o.placeStringBookmark();
                    "indent" === e ? this.oApp.exec("SET_LINE_STYLE", [null, jindo.$Fn(this._indentMargin, this).bind(), {
                        bDoNotSelect: !0,
                        bDontAddUndoHistory: !0
                    }]) : this.oApp.exec("SET_LINE_STYLE", [null, jindo.$Fn(this._outdentMargin, this).bind(), {
                        bDoNotSelect: !0,
                        bDontAddUndoHistory: !0
                    }]), o.moveToStringBookmark(s), o.select(), o.removeStringBookmark(s), setTimeout(jindo.$Fn(function(e) {
                        this.oApp.exec("RECORD_UNDO_AFTER_ACTION", [e])
                    }, this).bind(e), 25);
                    break;
                case "justifyleft":
                case "justifycenter":
                case "justifyright":
                case "justifyfull":
                    var r = this._extendBlock();
                    this.oEditingArea.execCommand(e, t, i), r && r.select(), this.oNavigator.ie && this._removeElementAlign("table");
                    break;
                default:
                    if (o.collapsed && this._rxCmdInline.test(e)) {
                        var a = o.placeStringBookmark(),
                            l = o.getStringBookmark(a).previousSibling;
                        l && "​" === l.nodeValue || (l = this.oApp.getWYSIWYGDocument().createTextNode("​"), o.insertNode(l)), o.removeStringBookmark(a), o.selectNodeContents(l), o.select(), this.oEditingArea.execCommand(e, t, i), o.collapseToEnd(), o.select();
                        var h = this._findSingleNode(l);
                        h && o._hasCursorHolderOnly(h.nextSibling) && h.parentNode.removeChild(h.nextSibling)
                    } else this.oEditingArea.execCommand(e, t, i)
            }
            this._countClickCr(e)
        },
        _findSingleNode: function(e) {
            return e ? 1 === e.parentNode.childNodes.length ? this._findSingleNode(e.parentNode) : e : null
        },
        $AFTER_EXECCOMMAND: function(e, t, i, n) {
            this.elP1 && this.elP1.parentNode && this.elP1.parentNode.removeChild(this.elP1), this.elP2 && this.elP2.parentNode && this.elP2.parentNode.removeChild(this.elP2), /^insertorderedlist|insertunorderedlist$/i.test(e) && (this._removeDummyParagraph_IE(), this._fixCorruptedBlockQuote("insertorderedlist" === e ? "OL" : "UL"), this.oNavigator.bGalaxyBrowser && this._removeBlockQuote()), /^justify*/i.test(e) && this._fixAlign("justifyfull" === e ? "justify" : e.substring(7)), "indent" != e && "outdent" != e || ((n = n || {}).bDontAddUndoHistory = !0), n && n.bDontAddUndoHistory || this._bOnlyCursorChanged || this.oApp.exec("RECORD_UNDO_AFTER_ACTION", [e, this.oUndoOption]), this.oApp.exec("CHECK_STYLE_CHANGE", [])
        },
        _removeElementAlign: function(e) {
            for (var t = this.oApp.getSelection().getNodes(), i = null, n = new RegExp("^" + e + "$", "i"), o = 0, s = t.length; o < s; o++)(i = t[o]).tagName && n.test(i.tagName) && (i.style.textAlign = "", i.removeAttribute("align"))
        },
        _getAlignNode: function(e) {
            if (e.tagName && ("P" === e.tagName || "DIV" === e.tagName)) return e;
            for (e = e.parentNode; e && e.tagName;) {
                if ("P" === e.tagName || "DIV" === e.tagName) return e;
                e = e.parentNode
            }
        },
        _fixAlign: function(e) {
            var t = this.oApp.getSelection(),
                i = [],
                n = null,
                o = null;
            t.collapsed ? i[0] = t.startContainer : i = t.getNodes();
            for (var s = 0, r = i.length; s < r; s++) 3 === (n = i[s]).nodeType && (n = n.parentNode), o && (n === o || jindo.$Element(n).isChildOf(o)) || (o = this._getAlignNode(n)) && o.align !== o.style.textAlign && (o.style.textAlign = e, o.setAttribute("align", e))
        },
        _getDocumentBR: function() {
            var e, t;
            for (this.aBRs = this.oApp.getWYSIWYGDocument().getElementsByTagName("BR"), this.aBeforeBRs = [], e = 0, t = this.aBRs.length; e < t; e++) this.aBeforeBRs[e] = this.aBRs[e]
        },
        _fixDocumentBR: function() {
            if (this.aBeforeBRs.length !== this.aBRs.length) {
                var e, t = jindo.$A(this.aBeforeBRs);
                for (e = this.aBRs.length - 1; 0 <= e; e--) t.indexOf(this.aBRs[e]) < 0 && this.aBRs[e].parentNode.removeChild(this.aBRs[e])
            }
        },
        _setBlockExecCommand: function(e, t, i) {
            var n, o, s = {};
            this.oSelection = this.oApp.getSelection(), this.oApp.exec("GET_SELECTED_TD_BLOCK", ["aTdCells", s]), n = s.aTdCells;
            for (var r = 0; r < n.length; r++) {
                this.oSelection.selectNodeContents(n[r]), this.oSelection.select(), this.oNavigator.firefox && this.oEditingArea.execCommand("styleWithCSS", t, !1), o = this.oSelection.getNodes();
                for (var a = 0; a < o.length; a++) "UL" != o[a].tagName && "OL" != o[a].tagName || jindo.$Element(o[a]).css("color", i);
                this.oEditingArea.execCommand(e, t, i)
            }
        },
        _indentMargin: function(e) {
            for (var t, i, n, o, s, r, a = e; a;) {
                if (a.tagName && "LI" === a.tagName) {
                    e = a;
                    break
                }
                a = a.parentNode
            }
            if ("LI" === e.tagName) {
                if (e.previousSibling && e.previousSibling.tagName && e.previousSibling.tagName === e.parentNode.tagName) {
                    if (e.nextSibling && e.nextSibling.tagName && e.nextSibling.tagName === e.parentNode.tagName) {
                        for (t = [e], i = 0, n = e.nextSibling.childNodes.length; i < n; i++) t.push(e.nextSibling.childNodes[i]);
                        for (o = e.previousSibling, s = e.nextSibling, i = 0, n = t.length; i < n; i++) o.insertBefore(t[i], null);
                        s.parentNode.removeChild(s)
                    } else e.previousSibling.insertBefore(e, null);
                    return
                }
                return e.nextSibling && e.nextSibling.tagName && e.nextSibling.tagName === e.parentNode.tagName ? void e.nextSibling.insertBefore(e, e.nextSibling.firstChild) : (a = e.parentNode.cloneNode(!1), e.parentNode.insertBefore(a, e), void a.appendChild(e))
            }
            r = (r = parseInt(e.style.marginLeft, 10)) || 0, r += this.nIndentSpacing, e.style.marginLeft = r + "px"
        },
        _outdentMargin: function(e) {
            for (var t, i, n, o, s, r = e; r;) {
                if (r.tagName && "LI" === r.tagName) {
                    e = r;
                    break
                }
                r = r.parentNode
            }
            if ("LI" !== e.tagName) s = (s = parseInt(e.style.marginLeft, 10)) || 0, (s -= this.nIndentSpacing) < 0 && (s = 0), e.style.marginLeft = s + "px";
            else {
                if (t = e.parentNode, i = e.parentNode, e.previousSibling && e.previousSibling.tagName && e.previousSibling.tagName.match(/LI|UL|OL/))
                    if (e.nextSibling && e.nextSibling.tagName && e.nextSibling.tagName.match(/LI|UL|OL/)) {
                        for (n = t.cloneNode(!1); e.nextSibling;) n.insertBefore(e.nextSibling, null);
                        t.parentNode.insertBefore(n, t.nextSibling), i = n
                    } else i = t.nextSibling;
                if (t.parentNode.insertBefore(e, i), t.innerHTML.match(/LI/i) || t.parentNode.removeChild(t), !e.parentNode.tagName.match(/OL|UL/)) {
                    for (o = e.parentNode, i = e, o = this.oApp.getWYSIWYGDocument().createElement("P"), i = null, e.parentNode.insertBefore(o, e); e.firstChild;) o.insertBefore(e.firstChild, i);
                    e.parentNode.removeChild(e)
                }
            }
        },
        _insertBlankLine: function() {
            var e = this.oApp.getSelection(),
                t = e.commonAncestorContainer;
            for (this.elP1 = null, this.elP2 = null; t;) {
                if ("BLOCKQUOTE" == t.tagName) {
                    this.elP1 = jindo.$("<p>&nbsp;</p>", this.oApp.getWYSIWYGDocument()), t.parentNode.insertBefore(this.elP1, t), this.elP2 = jindo.$("<p>&nbsp;</p>", this.oApp.getWYSIWYGDocument()), t.parentNode.insertBefore(this.elP2, t.nextSibling);
                    break
                }
                t = t.parentNode
            }
            if (!this.elP1 && !this.elP2) {
                var i = (t = 1 !== (t = e.commonAncestorContainer).nodeType ? t.parentNode : t).previousSibling,
                    n = t.nextSibling;
                i && "BLOCKQUOTE" === i.tagName && (this.elP1 = jindo.$("<p>&nbsp;</p>", this.oApp.getWYSIWYGDocument()), i.parentNode.insertBefore(this.elP1, i.nextSibling)), n && "BLOCKQUOTE" === n.tagName && (this.elP1 = jindo.$("<p>&nbsp;</p>", this.oApp.getWYSIWYGDocument()), n.parentNode.insertBefore(this.elP1, n))
            }
        },
        _fixCorruptedBlockQuote: function(e) {
            var t, i, n, o, s, r, a, l, h, d = this.oApp.getWYSIWYGDocument().getElementsByTagName(e);
            for (s = 0, r = d.length; s < r; s++)
                if (d[s].firstChild && d[s].firstChild.tagName == e) {
                    t = d[s];
                    break
                } if (t) {
                for (i = t.parentNode, a = this._getPosIdx(t), (l = this.oApp.getWYSIWYGDocument().createElement("DIV")).innerHTML = t.outerHTML.replace("<" + e, "<BLOCKQUOTE"), t.parentNode.insertBefore(l.firstChild, t), t.parentNode.removeChild(t), s = 0, r = (o = (n = i.childNodes[a]).getElementsByTagName(e)).length; s < r; s++) o[s].childNodes.length < 1 && o[s].parentNode.removeChild(o[s]);
                (h = this.oApp.getEmptySelection()).selectNodeContents(n), h.collapseToEnd(), h.select()
            }
        },
        _removeBlockQuote: function() {
            for (var e, t, i, n = "Apple-style-span", o = this.oApp.getSelection(), s = o.commonAncestorContainer, r = s; s;) s = "LI" === s.tagName ? "display: inline !important; " === (i = s).style.cssText ? s.parentNode : null : "SPAN" === s.tagName && s.className === n ? (e = s, i ? null : s.parentNode) : s.parentNode;
            for (i && e && ((s = e.parentNode).replaceChild(r, e), o.selectNodeContents(s), o.collapseToEnd(), o.select()); s;)
                if ("BLOCKQUOTE" === s.tagName) {
                    t = s.getElementsByClassName(n);
                    for (var a = 0; e = t[a]; a++) e.parentNode.removeChild(e);
                    s = null
                } else s = s.parentNode
        },
        _getPosIdx: function(e) {
            for (var t = 0, i = e.previousSibling; i; i = i.previousSibling) t++;
            return t
        },
        _countClickCr: function(e) {
            e.match(this.rxClickCr) && this.oApp.exec("MSG_NOTIFY_CLICKCR", [e.replace(/^insert/i, "")])
        },
        _extendBlock: function() {
            var e, t, i = this.oApp.getSelection(),
                n = i.startContainer,
                o = i.endContainer,
                s = i.cloneRange();
            if (n === o && 1 === n.nodeType && "P" === n.tagName && (e = jindo.$A(n.childNodes).filter(function(e) {
                    return 1 === e.nodeType && "IMG" === e.tagName
                }).$value(), t = jindo.$A(i.getNodes()).filter(function(e) {
                    return 1 === e.nodeType && "IMG" === e.tagName
                }).$value(), !(e.length <= t.length))) return i.selectNode(n), i.select(), s
        }
    })
}, function(e, t) {
    nhn.husky.SE_WYSIWYGStyler = jindo.$Class({
        name: "SE_WYSIWYGStyler",
        _sCursorHolder: "\ufeff",
        $init: function() {
            var e = jindo.$Agent().navigator();
            e.ie && 8 < e.version && (this._sCursorHolder = "⁠", this.$ON_REGISTER_CONVERTERS = function() {
                var t = /\u2060/g;
                this.oApp.exec("ADD_CONVERTER", ["WYSIWYG_TO_IR", jindo.$Fn(function(e) {
                    return e.replace(t, "\ufeff")
                }, this).bind()])
            })
        },
        $PRECONDITION: function() {
            return "WYSIWYG" == this.oApp.getEditingMode()
        },
        $ON_SET_WYSIWYG_STYLE: function(e) {
            var t = this.oApp.getSelection(),
                i = {};
            this.oApp.exec("IS_SELECTED_TD_BLOCK", ["bIsSelectedTd", i]);
            var n = i.bIsSelectedTd;
            if (t.collapsed && !n) {
                this.oApp.exec("RECORD_UNDO_ACTION", ["FONT STYLE", {
                    bMustBlockElement: !0
                }]);
                var o, s, r = !1,
                    a = t.commonAncestorContainer;
                for (var l in 3 == a.nodeType && (a = a.parentNode), a && t._rxCursorHolder.test(a.innerHTML) && (o = t._findParentSingleSpan(a)), o ? "" == o.innerHTML && (o.innerHTML = this._sCursorHolder) : ((o = this.oApp.getWYSIWYGDocument().createElement("SPAN")).innerHTML = this._sCursorHolder, r = !0), e) "string" == typeof(s = e[l]) && (o.style[l] = s);
                if (r)
                    if ("BODY" == t.startContainer.tagName && 0 === t.startOffset) {
                        var h = t._getVeryFirstRealChild(this.oApp.getWYSIWYGDocument().body),
                            d = !0,
                            c = h.cloneNode(!1);
                        try {
                            (c.innerHTML = "test") != c.innerHTML && (d = !1)
                        } catch (m) {
                            d = !1
                        }
                        d && 1 == c.nodeType && "BR" == c.tagName ? (t.selectNode(h), t.collapseToStart(), t.insertNode(o)) : d && "IFRAME" != h.tagName && h.appendChild && "string" == typeof h.innerHTML ? h.appendChild(o) : (t.selectNode(h), t.collapseToStart(), t.insertNode(o))
                    } else t.collapseToStart(), t.insertNode(o);
                else t = this.oApp.getEmptySelection();
                return e.color && t._checkTextDecoration(o), t.selectNodeContents(r ? o : a), t.collapseToEnd(), t._window.focus(), t._window.document.body.focus(), void t.select()
            }
            if (this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["FONT STYLE", {
                    bMustBlockElement: !0
                }]), n) {
                var p;
                this.oApp.exec("GET_SELECTED_TD_BLOCK", ["aTdCells", i]), p = i.aTdCells;
                for (var u = 0; u < p.length; u++) t.selectNodeContents(p[u]), t.styleRange(e), t.select()
            } else {
                var _ = !!e.color,
                    E = e.fontSize || e.fontFamily;
                p = t.getNodes();
                for (var g, f = 0; g = p[f]; f++) nhn.husky.SE2M_Utils.removeInvalidNodeInTable(g);
                if (t.styleRange(e, null, null, E, _), jindo.$Agent().navigator().firefox) {
                    var A = t.aStyleParents,
                        S = A.length;
                    for (f = 0; f < S; f++) {
                        var T = A[f];
                        T.nextSibling && "BR" == T.nextSibling.tagName && !T.nextSibling.nextSibling && T.parentNode.removeChild(T.nextSibling)
                    }
                }
                t._window.focus(), t.select()
            }
            this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["FONT STYLE", {
                bMustBlockElement: !0
            }])
        }
    })
}, function(e, t) {
    nhn.husky.SE_WYSIWYGStyleGetter = jindo.$Class({
        name: "SE_WYSIWYGStyleGetter",
        hKeyUp: null,
        getStyleInterval: 200,
        oStyleMap: {
            fontFamily: {
                type: "Value",
                css: "fontFamily"
            },
            fontSize: {
                type: "Value",
                css: "fontSize"
            },
            lineHeight: {
                type: "Value",
                css: "lineHeight",
                converter: function(e, t) {
                    return e.match(/px$/) ? Math.ceil(parseInt(e, 10) / parseInt(t.fontSize, 10) * 10) / 10 : e
                }
            },
            bold: {
                command: "bold"
            },
            underline: {
                command: "underline"
            },
            italic: {
                command: "italic"
            },
            lineThrough: {
                command: "strikethrough"
            },
            superscript: {
                command: "superscript"
            },
            subscript: {
                command: "subscript"
            },
            justifyleft: {
                command: "justifyleft"
            },
            justifycenter: {
                command: "justifycenter"
            },
            justifyright: {
                command: "justifyright"
            },
            justifyfull: {
                command: "justifyfull"
            },
            orderedlist: {
                command: "insertorderedlist"
            },
            unorderedlist: {
                command: "insertunorderedlist"
            }
        },
        $init: function() {
            this.oStyle = this._getBlankStyle()
        },
        $LOCAL_BEFORE_ALL: function() {
            return "WYSIWYG" == this.oApp.getEditingMode()
        },
        $ON_MSG_APP_READY: function() {
            this.oDocument = this.oApp.getWYSIWYGDocument(), this.oApp.exec("ADD_APP_PROPERTY", ["getCurrentStyle", jindo.$Fn(this.getCurrentStyle, this).bind()]), (jindo.$Agent().navigator().safari || jindo.$Agent().navigator().chrome || jindo.$Agent().navigator().ie) && (this.oStyleMap.textAlign = {
                type: "Value",
                css: "textAlign"
            })
        },
        $ON_EVENT_EDITING_AREA_MOUSEUP: function() {
            this.oApp.exec("CHECK_STYLE_CHANGE")
        },
        $ON_EVENT_EDITING_AREA_KEYPRESS: function(e) {
            var t;
            this.oApp.oNavigator.firefox && (t = e.key()).ctrl && 97 == t.keyCode || (this.bAllSelected ? this.bAllSelected = !1 : this.oApp.exec("CHECK_STYLE_CHANGE"))
        },
        $ON_EVENT_EDITING_AREA_KEYDOWN: function(e) {
            var t = e.key();
            if ((this.oApp.oNavigator.ie || this.oApp.oNavigator.firefox) && t.ctrl && 65 == t.keyCode) return this.oApp.exec("RESET_STYLE_STATUS"), void(this.bAllSelected = !0);
            if (8 == t.keyCode || 33 <= t.keyCode && t.keyCode <= 40 || 45 == t.keyCode || 46 == t.keyCode) {
                if (t.shift && 35 === t.keyCode) return this.oApp.exec("RESET_STYLE_STATUS"), void(this.bAllSelected = !0);
                if (this.bAllSelected) {
                    if (this.oApp.oNavigator.firefox) return;
                    this.bAllSelected = !1
                } else this.oApp.exec("CHECK_STYLE_CHANGE")
            }
        },
        $ON_CHECK_STYLE_CHANGE: function() {
            this._getStyle()
        },
        $ON_RESET_STYLE_STATUS: function() {
            this.oStyle = this._getBlankStyle();
            var e = this._getStyleOf(this.oApp.getWYSIWYGDocument().body);
            for (var t in this.oStyle.fontFamily = e.fontFamily, this.oStyle.fontSize = e.fontSize, this.oStyle.justifyleft = "@^", this.oStyle) this.oApp.exec("MSG_STYLE_CHANGED", [t, this.oStyle[t]])
        },
        getCurrentStyle: function() {
            return this.oStyle
        },
        _check_style_change: function() {
            this.oApp.exec("CHECK_STYLE_CHANGE", [])
        },
        _getBlankStyle: function() {
            var e = {};
            for (var t in this.oStyleMap) "Value" == this.oStyleMap[t].type ? e[t] = "" : e[t] = 0;
            return e
        },
        _getStyle: function() {
            var e;
            if (nhn.CurrentSelection.isCollapsed()) e = this._getStyleOf(nhn.CurrentSelection.getCommonAncestorContainer());
            else {
                var t = this.oApp.getSelection(),
                    i = t.getNodes(!1, function(e) {
                        return !e.childNodes || 0 == e.childNodes.length
                    });
                e = 0 == i.length ? this._getStyleOf(t.commonAncestorContainer) : this._getStyleOf(i[0])
            }
            for (var n in e)
                if (this.oStyleMap[n].converter && (e[n] = this.oStyleMap[n].converter(e[n], e)), this.oStyle[n] != e[n]) {
                    if ("object" != typeof document.body.currentStyle && "function" == typeof getComputedStyle && "fontSize" == n && this.oStyle.fontFamily != e.fontFamily) continue;
                    this.oApp.exec("MSG_STYLE_CHANGED", [n, e[n]])
                } this.oStyle = e
        },
        _getStyleOf: function(e) {
            var t = this._getBlankStyle();
            if (!e) return t;
            3 == e.nodeType ? e = e.parentNode : 9 == e.nodeType && (e = e.body);
            var i, n = jindo.$Element(e);
            for (var o in this.oStyle)
                if ((i = this.oStyleMap[o]).type && "Value" == i.type) try {
                    if (i.css) {
                        var s = n.css(i.css);
                        "fontFamily" == o && (s = s.split(/,/)[0]), t[o] = s
                    } else i.command && (t[o] = this.oDocument.queryCommandState(i.command))
                } catch (r) {} else if (i.command) try {
                    this.oDocument.queryCommandState(i.command) ? t[o] = "@^" : t[o] = "@-"
                } catch (r) {}
            switch (t.textAlign) {
                case "left":
                    t.justifyleft = "@^";
                    break;
                case "center":
                    t.justifycenter = "@^";
                    break;
                case "right":
                    t.justifyright = "@^";
                    break;
                case "justify":
                    t.justifyfull = "@^"
            }
            return "@-" == t.justifyleft && "@-" == t.justifycenter && "@-" == t.justifyright && "@-" == t.justifyfull && (t.justifyleft = "@^"), t
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_FontSizeWithLayerUI = jindo.$Class({
        name: "SE2M_FontSizeWithLayerUI",
        $init: function(e) {
            this._assignHTMLElements(e)
        },
        _assignHTMLElements: function(e) {
            this.oDropdownLayer = jindo.$$.getSingle("DIV.husky_se_fontSize_layer", e), this.elFontSizeLabel = jindo.$$.getSingle("SPAN.husky_se2m_current_fontSize", e), this.aLIFontSizes = jindo.$A(jindo.$$("LI", this.oDropdownLayer)).filter(function(e) {
                return null != e.firstChild
            })._array, this.sDefaultText = this.elFontSizeLabel.innerHTML
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.exec("REGISTER_UI_EVENT", ["fontSize", "click", "SE2M_TOGGLE_FONTSIZE_LAYER"]), this.oApp.exec("SE2_ATTACH_HOVER_EVENTS", [this.aLIFontSizes]);
            for (var e = 0; e < this.aLIFontSizes.length; e++) this.oApp.registerBrowserEvent(this.aLIFontSizes[e], "click", "SET_FONTSIZE", [this._getFontSizeFromLI(this.aLIFontSizes[e])])
        },
        $ON_SE2M_TOGGLE_FONTSIZE_LAYER: function() {
            this.oApp.exec("TOGGLE_TOOLBAR_ACTIVE_LAYER", [this.oDropdownLayer, null, "SELECT_UI", ["fontSize"], "DESELECT_UI", ["fontSize"]]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["size"])
        },
        _rxPX: /px$/i,
        _rxPT: /pt$/i,
        $ON_MSG_STYLE_CHANGED: function(e, t) {
            if ("fontSize" == e) {
                if (this._rxPX.test(t)) {
                    var i = parseFloat(t.replace(this._rxPX, ""));
                    t = 0 < i ? 72 * i / 96 + "pt" : this.sDefaultText
                }
                if (this._rxPT.test(t)) {
                    i = parseFloat(t.replace(this._rxPT, ""));
                    var n = Math.floor(i),
                        o = i - n;
                    t = (i = 0 <= o && o < .25 ? n + 0 : .25 <= o && o < .75 ? n + .5 : n + 1) + "pt"
                }
                t = t || this.sDefaultText;
                var s = this._getMatchingLI(t);
                this._clearFontSizeSelection(), s ? (this.elFontSizeLabel.innerHTML = t, jindo.$Element(s).addClass("active")) : this.elFontSizeLabel.innerHTML = t
            }
        },
        $ON_SET_FONTSIZE: function(e) {
            e && (this.oApp.exec("SET_WYSIWYG_STYLE", [{
                fontSize: e
            }]), this.oApp.exec("HIDE_ACTIVE_LAYER", []), this.oApp.exec("CHECK_STYLE_CHANGE", []))
        },
        _getMatchingLI: function(e) {
            var t;
            e = e.toLowerCase();
            for (var i = 0; i < this.aLIFontSizes.length; i++)
                if (t = this.aLIFontSizes[i], this._getFontSizeFromLI(t).toLowerCase() == e) return t;
            return null
        },
        _getFontSizeFromLI: function(e) {
            return e.firstChild.firstChild.style.fontSize
        },
        _clearFontSizeSelection: function() {
            for (var e = 0; e < this.aLIFontSizes.length; e++) jindo.$Element(this.aLIFontSizes[e]).removeClass("active")
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_LineStyler = jindo.$Class({
        name: "SE2M_LineStyler",
        $BEFORE_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["getLineStyle", jindo.$Fn(this.getLineStyle, this).bind()])
        },
        $ON_SET_LINE_STYLE: function(e, t, i) {
            this.oSelection = this.oApp.getSelection();
            var n = this._getSelectedNodes(!1);
            this.setLineStyle(e, t, i, n), this.oApp.exec("CHECK_STYLE_CHANGE", [])
        },
        $ON_SET_LINE_BLOCK_STYLE: function(e, t, i) {
            this.oSelection = this.oApp.getSelection(), this.setLineBlockStyle(e, t, i), this.oApp.exec("CHECK_STYLE_CHANGE", [])
        },
        _getSelectedTDs: function() {
            var e = {};
            return this.oApp.exec("GET_SELECTED_TD_BLOCK", ["aTdCells", e]), e.aTdCells || []
        },
        getLineStyle: function(e) {
            var t, i, n, o = this._getSelectedNodes(!1);
            if (0 === o.length) return null;
            var s = o.length;
            n = 0 === s ? null : (i = this._getLineWrapper(o[0]), this._getWrapperLineStyle(e, i));
            var r = this.oSelection.getStartNode();
            if (null != n)
                for (var a = 1; a < s; a++)
                    if (!this._isChildOf(o[a], t) && o[a] && (t = this._getLineWrapper(o[a])) != i) {
                        if (this._getWrapperLineStyle(e, t) != n) {
                            n = null;
                            break
                        }
                        i = t
                    } t = this._getLineWrapper(o[s - 1]);
            var l = this.oSelection.getEndNode();
            return setTimeout(jindo.$Fn(function(e, t) {
                var i = this._getSelectedTDs();
                if (0 < i.length) {
                    var n = nhn.husky.SE2M_Utils.findAncestorByTagName("TD", e),
                        o = nhn.husky.SE2M_Utils.findAncestorByTagName("TD", t);
                    e = n || !e ? i[0].firstChild : e, t = o || !t ? i[i.length - 1].lastChild : t
                }
                this.oSelection.setEndNodes(e, t), this.oSelection.select(), this.oApp.exec("CHECK_STYLE_CHANGE", [])
            }, this).bind(r, l), 0), n
        },
        setLineStyle: function(e, t, i, n) {
            var o = this,
                s = !1;

            function r(e, t, i) {
                if (!e) {
                    s = !0;
                    try {
                        e = o.oSelection.surroundContentsWithNewNode("P")
                    } catch (n) {
                        e = o.oSelection.surroundContentsWithNewNode("DIV")
                    }
                }
                return "function" == typeof i ? i(e) : e.style[t] = i, 0 === e.childNodes.length && (e.innerHTML = "&nbsp;"), e
            }

            function a(e) {
                for (; e && "BODY" != e.tagName;) e = nhn.DOMFix.parentNode(e);
                return !!e
            }
            if (0 !== n.length) {
                var l, h, d = n.length;
                i && i.bDontAddUndoHistory || this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["LINE STYLE"]);
                for (var c, p = h = r(h = this._getLineWrapper(n[0]), e, t), u = 1; u < d; u++) {
                    try {
                        if (!a(nhn.DOMFix.parentNode(n[u]))) continue
                    } catch (_) {
                        continue
                    }
                    this._isChildOf(n[u], l) || (l = this._getLineWrapper(n[u])) != h && (h = l = r(l, e, t))
                }
                c = l || p, !s || i && i.bDoNotSelect || setTimeout(jindo.$Fn(function(e, t, i) {
                    e == t ? (this.oSelection.selectNodeContents(e), 1 == e.childNodes.length && "BR" == e.firstChild.tagName && this.oSelection.collapseToStart()) : this.oSelection.setEndNodes(e, t), this.oSelection.select(), i && i.bDontAddUndoHistory || this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["LINE STYLE"])
                }, this).bind(p, c, i), 0)
            }
        },
        setLineBlockStyle: function(e, t, i) {
            for (var n = [], o = [], s = this._getSelectedTDs(), r = 0; r < s.length; r++) {
                this.oSelection.selectNode(s[r]), n = this.oSelection.getNodes();
                for (var a = 0, l = 0; a < n.length; a++)(3 == n[a].nodeType || "BR" == n[a].tagName && 0 == a) && (o[l] = n[a], l++);
                this.setLineStyle(e, t, i, o), n = o = []
            }
        },
        getTextNodes: function(e, t) {
            return t.getNodes(e, function(e) {
                return 3 == e.nodeType && "\n" != e.nodeValue && "" != e.nodeValue && "\ufeff" != e.nodeValue || "LI" == e.tagName && "" == e.innerHTML || "P" == e.tagName && "" == e.innerHTML
            })
        },
        _getSelectedNodes: function(e) {
            if (e || (this.oSelection = this.oApp.getSelection()), "LI" == this.oSelection.endContainer.tagName && 0 == this.oSelection.endOffset && "" == this.oSelection.endContainer.innerHTML && this.oSelection.setEndAfter(this.oSelection.endContainer), this.oSelection.collapsed) {
                var t = this._getSelectedTDs();
                if (0 < t.length) return [t[0].firstChild, t[t.length - 1].lastChild];
                this.oSelection.selectNode(this.oSelection.commonAncestorContainer)
            }
            var i = this.getTextNodes(!1, this.oSelection);
            if (0 === i.length) {
                var n = this.oSelection.getStartNode();
                if (n) i[0] = n;
                else {
                    var o = this.oSelection._document.createTextNode(" ");
                    this.oSelection.insertNode(o), i = [o]
                }
            }
            return i
        },
        _getWrapperLineStyle: function(e, t) {
            var i = null;
            if (t && t.style[e]) i = t.style[e];
            else
                for (t = this.oSelection.commonAncesterContainer; t && !this.oSelection.rxLineBreaker.test(t.tagName);) {
                    if (t && t.style[e]) {
                        i = t.style[e];
                        break
                    }
                    t = nhn.DOMFix.parentNode(t)
                }
            return i
        },
        _isChildOf: function(e, t) {
            for (; e && "BODY" != e.tagName;) {
                if (e == t) return !0;
                e = nhn.DOMFix.parentNode(e)
            }
            return !1
        },
        _getLineWrapper: function(e) {
            var t = this.oApp.getEmptySelection();
            t.selectNode(e);
            var i, n, o, s, r = t.getLineInfo(),
                a = r.oStart,
                l = r.oEnd,
                h = null;
            return i = a.oNode, o = a.oLineBreaker, n = l.oNode, s = l.oLineBreaker, this.oSelection.setEndNodes(i, n), o == s && ("P" == o.tagName || "DIV" == o.tagName || "LI" == o.tagName ? h = o : this.oSelection.setEndNodes(o.firstChild, o.lastChild)), h
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_LineHeightWithLayerUI = jindo.$Class({
        name: "SE2M_LineHeightWithLayerUI",
        MIN_LINE_HEIGHT: 50,
        $ON_MSG_APP_READY: function() {
            this.oApp.exec("REGISTER_UI_EVENT", ["lineHeight", "click", "SE2M_TOGGLE_LINEHEIGHT_LAYER"]), this.oApp.registerLazyMessage(["SE2M_TOGGLE_LINEHEIGHT_LAYER"], ["hp_SE2M_LineHeightWithLayerUI$Lazy.js"])
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_ColorPalette = jindo.$Class({
        name: "SE2M_ColorPalette",
        elAppContainer: null,
        bUseRecentColor: !1,
        nLimitRecentColor: 17,
        rxRGBColorPattern: /rgb\((\d+), ?(\d+), ?(\d+)\)/i,
        rxColorPattern: /^#?[0-9a-fA-F]{6}$|^rgb\(\d+, ?\d+, ?\d+\)$/i,
        aRecentColor: [],
        URL_COLOR_LIST: "",
        URL_COLOR_ADD: "",
        URL_COLOR_UPDATE: "",
        sRecentColorTemp: '<li><button type="button" title="{RGB_CODE}" style="background:{RGB_CODE}"><span><span>{RGB_CODE}</span></span></button></li>',
        $init: function(e) {
            this.elAppContainer = e
        },
        $ON_MSG_APP_READY: function() {},
        _assignHTMLElements: function(e) {
            var t = nhn.husky.SE2M_Configuration.SE2M_ColorPalette;
            if (t) {
                var i = (nhn.husky.SE2M_Configuration.LinkageDomain || {}).sCommonAPI || "";
                this.bUseRecentColor = t.bUseRecentColor || !1, this.URL_COLOR_ADD = t.addColorURL || i + "/1/colortable/TextAdd.nhn", this.URL_COLOR_UPDATE = t.updateColorURL || i + "/1/colortable/TextUpdate.nhn", this.URL_COLOR_LIST = t.colorListURL || i + "/1/colortable/TextList.nhn"
            }
            this.elColorPaletteLayer = jindo.$$.getSingle("DIV.husky_se2m_color_palette", e), this.elColorPaletteLayerColorPicker = jindo.$$.getSingle("DIV.husky_se2m_color_palette_colorpicker", this.elColorPaletteLayer), this.elRecentColorForm = jindo.$$.getSingle("form", this.elColorPaletteLayerColorPicker), this.elBackgroundColor = jindo.$$.getSingle("ul.husky_se2m_bgcolor_list", e), this.elInputColorCode = jindo.$$.getSingle("INPUT.husky_se2m_cp_colorcode", this.elColorPaletteLayerColorPicker), this.elPreview = jindo.$$.getSingle("SPAN.husky_se2m_cp_preview", this.elColorPaletteLayerColorPicker), this.elCP_ColPanel = jindo.$$.getSingle("DIV.husky_se2m_cp_colpanel", this.elColorPaletteLayerColorPicker), this.elCP_HuePanel = jindo.$$.getSingle("DIV.husky_se2m_cp_huepanel", this.elColorPaletteLayerColorPicker), this.elCP_ColPanel.style.position = "relative", this.elCP_HuePanel.style.position = "relative", this.elColorPaletteLayerColorPicker.style.display = "none", this.elMoreBtn = jindo.$$.getSingle("BUTTON.husky_se2m_color_palette_more_btn", this.elColorPaletteLayer), this.welMoreBtn = jindo.$Element(this.elMoreBtn), this.elOkBtn = jindo.$$.getSingle("BUTTON.husky_se2m_color_palette_ok_btn", this.elColorPaletteLayer), this.bUseRecentColor && (this.elColorPaletteLayerRecent = jindo.$$.getSingle("DIV.husky_se2m_color_palette_recent", this.elColorPaletteLayer), this.elRecentColor = jindo.$$.getSingle("ul.se2_pick_color", this.elColorPaletteLayerRecent), this.elDummyNode = jindo.$$.getSingle("ul.se2_pick_color > li", this.elColorPaletteLayerRecent) || null, this.elColorPaletteLayerRecent.style.display = "none")
        },
        $LOCAL_BEFORE_FIRST: function() {
            this._assignHTMLElements(this.elAppContainer), this.elDummyNode && jindo.$Element(jindo.$$.getSingle("ul.se2_pick_color > li", this.elColorPaletteLayerRecent)).leave(), this.bUseRecentColor && this._ajaxRecentColor(this._ajaxRecentColorCallback), this.oApp.registerBrowserEvent(this.elColorPaletteLayer, "click", "EVENT_CLICK_COLOR_PALETTE"), this.oApp.bMobile || (this.oApp.registerBrowserEvent(this.elBackgroundColor, "mouseover", "EVENT_MOUSEOVER_COLOR_PALETTE"), this.oApp.registerBrowserEvent(this.elColorPaletteLayer, "mouseover", "EVENT_MOUSEOVER_COLOR_PALETTE"), this.oApp.registerBrowserEvent(this.elBackgroundColor, "mouseout", "EVENT_MOUSEOUT_COLOR_PALETTE"), this.oApp.registerBrowserEvent(this.elColorPaletteLayer, "mouseout", "EVENT_MOUSEOUT_COLOR_PALETTE"))
        },
        $ON_EVENT_MOUSEOVER_COLOR_PALETTE: function(e) {
            for (var t = e.element; t && t.tagName && "li" != t.tagName.toLowerCase();) t = t.parentNode;
            t && t.nodeType && 9 != t.nodeType && ("" != t.className && t.className && "undefined" != typeof t.className || jindo.$Element(t).addClass("hover"))
        },
        $ON_EVENT_MOUSEOUT_COLOR_PALETTE: function(e) {
            for (var t = e.element; t && t.tagName && "li" != t.tagName.toLowerCase();) t = t.parentNode;
            t && "hover" == t.className && jindo.$Element(t).removeClass("hover")
        },
        $ON_EVENT_CLICK_COLOR_PALETTE: function(e) {
            for (var t = e.element;
                "SPAN" == t.tagName;) t = t.parentNode;
            if (t.tagName && "BUTTON" == t.tagName) {
                if (t == this.elMoreBtn) return void this.oApp.exec("TOGGLE_COLOR_PICKER");
                this.oApp.exec("APPLY_COLOR", [t])
            }
        },
        $ON_APPLY_COLOR: function(e) {
            var t = this.elInputColorCode.value,
                i = null;
            if (-1 == t.indexOf("#") && (t = "#" + t, this.elInputColorCode.value = t), e == this.elOkBtn) return this._verifyColorCode(t) ? void this.oApp.exec("COLOR_PALETTE_APPLY_COLOR", [t, !0]) : (this.elInputColorCode.value = "", alert(this.oApp.$MSG("SE_Color.invalidColorCode")), void this.elInputColorCode.focus());
            i = jindo.$Element(e.parentNode.parentNode.parentNode), t = e.title, i.hasClass("husky_se2m_color_palette") ? this.oApp.exec("COLOR_PALETTE_APPLY_COLOR", [t, !1]) : i.hasClass("husky_se2m_color_palette_recent") && this.oApp.exec("COLOR_PALETTE_APPLY_COLOR", [t, !0])
        },
        $ON_RESET_COLOR_PALETTE: function() {
            this._initColor()
        },
        $ON_TOGGLE_COLOR_PICKER: function() {
            "none" == this.elColorPaletteLayerColorPicker.style.display ? this.oApp.exec("SHOW_COLOR_PICKER") : this.oApp.exec("HIDE_COLOR_PICKER")
        },
        $ON_SHOW_COLOR_PICKER: function() {
            this.elColorPaletteLayerColorPicker.style.display = "", this.cpp = new nhn.ColorPicker(this.elCP_ColPanel, {
                huePanel: this.elCP_HuePanel
            });
            var e = jindo.$Fn(function(e) {
                this.elPreview.style.backgroundColor = e.hexColor, this.elInputColorCode.value = e.hexColor
            }, this).bind();
            this.cpp.attach("colorchange", e), this.$ON_SHOW_COLOR_PICKER = this._showColorPickerMain, this.$ON_SHOW_COLOR_PICKER()
        },
        $ON_HIDE_COLOR_PICKER: function() {
            this.elColorPaletteLayerColorPicker.style.display = "none", this.welMoreBtn.addClass("se2_view_more"), this.welMoreBtn.removeClass("se2_view_more2")
        },
        $ON_SHOW_COLOR_PALETTE: function(e, t) {
            this.sCallbackCmd = e, this.oLayerContainer = t, this.oLayerContainer.insertBefore(this.elColorPaletteLayer, null), this.elColorPaletteLayer.style.display = "block", this.oApp.delayedExec("POSITION_TOOLBAR_LAYER", [this.elColorPaletteLayer.parentNode.parentNode], 0)
        },
        $ON_HIDE_COLOR_PALETTE: function() {
            this.elColorPaletteLayer.style.display = "none"
        },
        $ON_COLOR_PALETTE_APPLY_COLOR: function(e, t) {
            t = t || !1, e = this._getHexColorCode(e), this.bUseRecentColor && t && this.oApp.exec("ADD_RECENT_COLOR", [e]), this.oApp.exec(this.sCallbackCmd, [e])
        },
        $ON_EVENT_MOUSEUP_COLOR_PALETTE: function(e) {
            var t = e.element;
            t.style.backgroundColor && this.oApp.exec("COLOR_PALETTE_APPLY_COLOR", [t.style.backgroundColor, !1])
        },
        $ON_ADD_RECENT_COLOR: function(e) {
            var t = 0 === this.aRecentColor.length;
            this._addRecentColor(e), t ? this._ajaxAddColor() : this._ajaxUpdateColor(), this._redrawRecentColorElement()
        },
        _verifyColorCode: function(e) {
            return this.rxColorPattern.test(e)
        },
        _getHexColorCode: function(e) {
            if (this.rxRGBColorPattern.test(e)) {
                function t(e) {
                    var t = parseInt(e, 10).toString(16);
                    return t.length < 2 && (t = "0" + t), t.toUpperCase()
                }
                e = "#" + t(RegExp.$1) + t(RegExp.$2) + t(RegExp.$3)
            }
            return e
        },
        _addRecentColor: function(e) {
            var t = jindo.$A(this.aRecentColor);
            (t = t.refuse(e)).unshift(e), t.length() > this.nLimitRecentColor && t.length(this.nLimitRecentColor), this.aRecentColor = t.$value()
        },
        _redrawRecentColorElement: function() {
            var e, t = [],
                i = this.aRecentColor.length;
            if (0 !== i) {
                for (e = 0; e < i; e++) t.push(this.sRecentColorTemp.replace(/\{RGB_CODE\}/gi, this.aRecentColor[e]));
                this.elRecentColor.innerHTML = t.join(""), this.elColorPaletteLayerRecent.style.display = "block"
            }
        },
        _ajaxAddColor: function() {
            jindo.$Ajax(this.URL_COLOR_ADD, {
                type: "jsonp",
                onload: function() {}
            }).request({
                text_key: "colortable",
                text_data: this.aRecentColor.join(",")
            })
        },
        _ajaxUpdateColor: function() {
            jindo.$Ajax(this.URL_COLOR_UPDATE, {
                type: "jsonp",
                onload: function() {}
            }).request({
                text_key: "colortable",
                text_data: this.aRecentColor.join(",")
            })
        },
        _showColorPickerMain: function() {
            this._initColor(), this.elColorPaletteLayerColorPicker.style.display = "", this.welMoreBtn.removeClass("se2_view_more"), this.welMoreBtn.addClass("se2_view_more2")
        },
        _initColor: function() {
            this.cpp && this.cpp.rgb({
                r: 0,
                g: 0,
                b: 0
            }), this.elPreview.style.backgroundColor = "#000000", this.elInputColorCode.value = "#000000", this.oApp.exec("HIDE_COLOR_PICKER")
        },
        _ajaxRecentColor: function(e) {
            jindo.$Ajax(this.URL_COLOR_LIST, {
                type: "jsonp",
                onload: jindo.$Fn(e, this).bind()
            }).request()
        },
        _ajaxRecentColorCallback: function(e) {
            var t, i, n, o = e.json().result;
            if (o && !o.error) {
                for ((t = jindo.$A(o).filter(this._verifyColorCode, this)).length() > this.nLimitRecentColor && t.length(this.nLimitRecentColor), i = 0, n = (o = t.reverse().$value()).length; i < n; i++) this._addRecentColor(this._getHexColorCode(o[i]));
                this._redrawRecentColorElement()
            }
        }
    }).extend(jindo.Component)
}, function(e, t) {
    nhn.husky.SE2M_FontColor = jindo.$Class({
        name: "SE2M_FontColor",
        rxColorPattern: /^#?[0-9a-fA-F]{6}$|^rgb\(\d+, ?\d+, ?\d+\)$/i,
        $init: function(e) {
            this._assignHTMLElements(e)
        },
        _assignHTMLElements: function(e) {
            this.elLastUsed = jindo.$$.getSingle("SPAN.husky_se2m_fontColor_lastUsed", e), this.elDropdownLayer = jindo.$$.getSingle("DIV.husky_se2m_fontcolor_layer", e), this.elPaletteHolder = jindo.$$.getSingle("DIV.husky_se2m_fontcolor_paletteHolder", this.elDropdownLayer), this._setLastUsedFontColor("#000000")
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["getLastUsedFontColor", jindo.$Fn(this.getLastUsedFontColor, this).bind()])
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.exec("REGISTER_UI_EVENT", ["fontColorA", "click", "APPLY_LAST_USED_FONTCOLOR"]), this.oApp.exec("REGISTER_UI_EVENT", ["fontColorB", "click", "TOGGLE_FONTCOLOR_LAYER"]), this.oApp.registerLazyMessage(["APPLY_LAST_USED_FONTCOLOR", "TOGGLE_FONTCOLOR_LAYER"], ["hp_SE2M_FontColor$Lazy.js"])
        },
        _setLastUsedFontColor: function(e) {
            this.sLastUsedColor = e, this.elLastUsed.style.backgroundColor = this.sLastUsedColor
        },
        getLastUsedFontColor: function() {
            return this.sLastUsedColor ? this.sLastUsedColor : "#000000"
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_BGColor = jindo.$Class({
        name: "SE2M_BGColor",
        rxColorPattern: /^#?[0-9a-fA-F]{6}$|^rgb\(\d+, ?\d+, ?\d+\)$/i,
        $init: function(e) {
            this._assignHTMLElements(e)
        },
        _assignHTMLElements: function(e) {
            this.elLastUsed = jindo.$$.getSingle("SPAN.husky_se2m_BGColor_lastUsed", e), this.elDropdownLayer = jindo.$$.getSingle("DIV.husky_se2m_BGColor_layer", e), this.elBGColorList = jindo.$$.getSingle("UL.husky_se2m_bgcolor_list", e), this.elPaletteHolder = jindo.$$.getSingle("DIV.husky_se2m_BGColor_paletteHolder", this.elDropdownLayer), this._setLastUsedBGColor("#777777")
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["getLastUsedBackgroundColor", jindo.$Fn(this.getLastUsedBGColor, this).bind()])
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.exec("REGISTER_UI_EVENT", ["BGColorA", "click", "APPLY_LAST_USED_BGCOLOR"]), this.oApp.exec("REGISTER_UI_EVENT", ["BGColorB", "click", "TOGGLE_BGCOLOR_LAYER"]), this.oApp.registerBrowserEvent(this.elBGColorList, "click", "EVENT_APPLY_BGCOLOR", []), this.oApp.registerLazyMessage(["APPLY_LAST_USED_BGCOLOR", "TOGGLE_BGCOLOR_LAYER"], ["hp_SE2M_BGColor$Lazy.js"])
        },
        _setLastUsedBGColor: function(e) {
            this.sLastUsedColor = e, this.elLastUsed.style.backgroundColor = this.sLastUsedColor
        },
        getLastUsedBGColor: function() {
            return this.sLastUsedColor ? this.sLastUsedColor : "#777777"
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_Hyperlink = jindo.$Class({
        name: "SE2M_Hyperlink",
        sATagMarker: "HTTP://HUSKY_TMP.MARKER/",
        _assignHTMLElements: function(e) {
            this.oHyperlinkButton = jindo.$$.getSingle("li.husky_seditor_ui_hyperlink", e), this.oHyperlinkLayer = jindo.$$.getSingle("div.se2_layer", this.oHyperlinkButton), this.oLinkInput = jindo.$$.getSingle("INPUT[type=text]", this.oHyperlinkLayer), this.oBtnConfirm = jindo.$$.getSingle("button.se2_apply", this.oHyperlinkLayer), this.oBtnCancel = jindo.$$.getSingle("button.se2_cancel", this.oHyperlinkLayer)
        },
        _generateAutoLink: function(e, t, i, n, o) {
            return (t = t || "") + (n ? '<a href="http://' + n + '">' + i + "</a>" : '<a href="' + o + '">' + i + "</a>")
        },
        $BEFORE_MSG_APP_READY: function() {
            var e = nhn.husky.SE2M_Configuration.SE2M_Hyperlink;
            if (e && !1 === e.bAutolink) {
                this.$ON_REGISTER_CONVERTERS = null, this.$ON_DISABLE_MESSAGE = null, this.$ON_ENABLE_MESSAGE = null;
                try {
                    this.oApp.getWYSIWYGDocument().execCommand("AutoUrlDetect", !1, !1)
                } catch (t) {}
            }
        },
        $ON_MSG_APP_READY: function() {
            this.bLayerShown = !1, jindo.$Agent().os().mac ? this.oApp.exec("REGISTER_HOTKEY", ["meta+k", "TOGGLE_HYPERLINK_LAYER", []]) : this.oApp.exec("REGISTER_HOTKEY", ["ctrl+k", "TOGGLE_HYPERLINK_LAYER", []]), this.oApp.exec("REGISTER_UI_EVENT", ["hyperlink", "click", "TOGGLE_HYPERLINK_LAYER"]), this.oApp.registerLazyMessage(["TOGGLE_HYPERLINK_LAYER", "APPLY_HYPERLINK"], ["hp_SE2M_Hyperlink$Lazy.js"])
        },
        $ON_REGISTER_CONVERTERS: function() {
            this.oApp.exec("ADD_CONVERTER_DOM", ["IR_TO_DB", jindo.$Fn(this.irToDb, this).bind()])
        },
        $LOCAL_BEFORE_FIRST: function(e) {
            if (e.match(/(REGISTER_CONVERTERS)/)) return this.oApp.acceptLocalBeforeFirstAgain(this, !0), !0;
            this._assignHTMLElements(this.oApp.htOptions.elAppContainer), this.sRXATagMarker = this.sATagMarker.replace(/\//g, "\\/").replace(/\./g, "\\."), this.oApp.registerBrowserEvent(this.oBtnConfirm, "click", "APPLY_HYPERLINK"), this.oApp.registerBrowserEvent(this.oBtnCancel, "click", "HIDE_ACTIVE_LAYER"), this.oApp.registerBrowserEvent(this.oLinkInput, "keydown", "EVENT_HYPERLINK_KEYDOWN")
        },
        $ON_EVENT_HYPERLINK_KEYDOWN: function(e) {
            e.key().enter && (this.oApp.exec("APPLY_HYPERLINK"), e.stop())
        },
        $ON_DISABLE_MESSAGE: function(e) {
            if ("TOGGLE_HYPERLINK_LAYER" === e) {
                try {
                    this.oApp.getWYSIWYGDocument().execCommand("AutoUrlDetect", !1, !1)
                } catch (t) {}
                this._bDisabled = !0
            }
        },
        $ON_ENABLE_MESSAGE: function(e) {
            if ("TOGGLE_HYPERLINK_LAYER" === e) {
                try {
                    this.oApp.getWYSIWYGDocument().execCommand("AutoUrlDetect", !1, !0)
                } catch (t) {}
                this._bDisabled = !1
            }
        },
        irToDb: function(e) {
            if (!this._bDisabled) {
                var t = e.cloneNode(!0);
                try {
                    t.innerHTML
                } catch (E) {
                    t = jindo.$(e.outerHTML)
                }
                for (var i, n, o = this.oApp.getEmptySelection(), s = o._getFirstRealChild(t), r = o._getLastRealChild(t), a = jindo.$A(o._getNodesBetween(s, r)), l = a.filter(function(e) {
                        return e && 3 === e.nodeType
                    }).$value(), h = l, d = this.oApp.getWYSIWYGDocument().createElement("DIV"), c = "@" + (new Date).getTime() + "@", p = new RegExp(c, "g"), u = 0, _ = l.length; u < _; u++) {
                    for (i = h[u].parentNode, n = !1; i;) {
                        if ("A" === i.tagName || "PRE" === i.tagName) {
                            n = !0;
                            break
                        }
                        i = i.parentNode
                    }
                    if (!n) {
                        d.innerHTML = "";
                        try {
                            d.appendChild(h[u].cloneNode(!0)), d.innerHTML = (c + d.innerHTML).replace(/(&nbsp|\s)?(((?!http[s]?:\/\/)www\.(?:(?!&nbsp;|\s|"|').)+)|(http[s]?:\/\/(?:(?!&nbsp;|\s|"|').)+))/gi, this._generateAutoLink), h[u].parentNode.insertBefore(d, h[u]), h[u].parentNode.removeChild(h[u])
                        } catch (g) {}
                        for (; d.firstChild;) d.parentNode.insertBefore(d.firstChild, d);
                        d.parentNode.removeChild(d)
                    }
                }
                d = o = s = r = a = l = h = i = null, t.innerHTML = t.innerHTML.replace(p, ""), e.innerHTML = t.innerHTML, t = null
            }
        }
    })
}, function(e, t) {
    function c(e, t, i, n, o) {
        this.fontId = e, this.fontName = t, this.defaultSize = i, this.fontURL = n, this.fontCSSURL = o, this.displayName = t, this.isLoaded = !0, this.fontFamily = this.fontId, "" != this.fontCSSURL ? (this.displayName += "" + i, this.fontFamily += "_" + i, this.isLoaded = !1, this.loadCSS = function(e) {
            this.isLoaded || (this._importCSS(e), this.isLoaded = !0)
        }, this.loadCSSToMenu = function() {
            this._importCSS(document)
        }, this._importCSS = function(e) {
            var t = e.styleSheets.length,
                i = e.styleSheets[t - 1];
            (0 === t || 30 < i.imports.length) && (i = e.createElement("style"), e.documentElement.firstChild.appendChild(i), i = i.sheet || i.styleSheet), i.addImport(this.fontCSSURL)
        }) : (this.loadCSS = function() {}, this.loadCSSToMenu = function() {}), this.toStruct = function() {
            return {
                fontId: this.fontId,
                fontName: this.fontName,
                defaultSize: this.defaultSize,
                fontURL: this.fontURL,
                fontCSSURL: this.fontCSSURL
            }
        }
    }
    nhn.husky.SE2M_FontNameWithLayerUI = jindo.$Class({
        name: "SE2M_FontNameWithLayerUI",
        FONT_SEPARATOR: "husky_seditor_font_separator",
        _rxQuote: /['"]/g,
        _rxComma: /\s*,\s*/g,
        $init: function(e, t) {
            this.elLastHover = null, this._assignHTMLElements(e), this.htBrowser = jindo.$Agent().navigator(), this.aAdditionalFontList = t || []
        },
        addAllFonts: function() {
            var e, t, i, n, o, s;
            if (this.htFamilyName2DisplayName = {}, this.htAllFonts = {}, this.aBaseFontList = [], this.aDefaultFontList = [], this.aTempSavedFontList = [], this.htOptions = this.oApp.htOptions.SE2M_FontName, this.htOptions) {
                if (e = this.htOptions.aDefaultFontList || [], t = this.htOptions.aFontList, i = this.htOptions.htMainFont, n = this.htOptions.aFontInUse, this.htBrowser.ie && t)
                    for (o = 0; o < t.length; o++) this.addFont(t[o].id, t[o].name, t[o].size, t[o].url, t[o].cssUrl);
                for (o = 0; o < e.length; o++) this.addFont(e[o][0], e[o][1], 0, "", "", 1);
                if (i && i.id && this.setMainFont(i.id, i.name, i.size, i.url, i.cssUrl), this.htBrowser.ie && n)
                    for (o = 0; o < n.length; o++) this.addFontInUse(n[o].id, n[o].name, n[o].size, n[o].url, n[o].cssUrl)
            }
            if (this.htOptions && this.htOptions.aDefaultFontList && 0 !== this.htOptions.aDefaultFontList.length || (this.addFont("돋움,Dotum", "돋움", 0, "", "", 1, null, !0), this.addFont("돋움체,DotumChe,AppleGothic", "돋움체", 0, "", "", 1, null, !0), this.addFont("굴림,Gulim", "굴림", 0, "", "", 1, null, !0), this.addFont("굴림체,GulimChe", "굴림체", 0, "", "", 1, null, !0), this.addFont("바탕,Batang,AppleMyungjo", "바탕", 0, "", "", 1, null, !0), this.addFont("바탕체,BatangChe", "바탕체", 0, "", "", 1, null, !0), this.addFont("궁서,Gungsuh,GungSeo", "궁서", 0, "", "", 1, null, !0), this.addFont("Arial", "Arial", 0, "", "", 1, "abcd", !0), this.addFont("Tahoma", "Tahoma", 0, "", "", 1, "abcd", !0), this.addFont("Times New Roman", "Times New Roman", 0, "", "", 1, "abcd", !0), this.addFont("Verdana", "Verdana", 0, "", "", 1, "abcd", !0), this.addFont("Courier New", "Courier New", 0, "", "", 1, "abcd", !0)), this.aAdditionalFontList && 0 < this.aAdditionalFontList.length)
                for (o = 0, s = this.aAdditionalFontList.length; o < s; o++) this.addFont(this.aAdditionalFontList[o][0], this.aAdditionalFontList[o][1], 0, "", "", 1)
        },
        $ON_MSG_APP_READY: function() {
            this.bDoNotRecordUndo = !1, this.oApp.exec("ADD_APP_PROPERTY", ["addFont", jindo.$Fn(this.addFont, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["addFontInUse", jindo.$Fn(this.addFontInUse, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["setMainFont", jindo.$Fn(this.setMainFont, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["setDefaultFont", jindo.$Fn(this.setDefaultFont, this).bind()]), this.oApp.exec("REGISTER_UI_EVENT", ["fontName", "click", "SE2M_TOGGLE_FONTNAME_LAYER"]), this._initFontName()
        },
        $AFTER_MSG_APP_READY: function() {
            this._attachIEEvent()
        },
        _assignHTMLElements: function(e) {
            this.oDropdownLayer = jindo.$$.getSingle("DIV.husky_se_fontName_layer", e), this.elFontNameLabel = jindo.$$.getSingle("SPAN.husky_se2m_current_fontName", e), this.elFontNameList = jindo.$$.getSingle("UL", this.oDropdownLayer), this.elInnerLayer = this.elFontNameList.parentNode, this.aelFontInMarkup = jindo.$$("LI", this.oDropdownLayer), this.elFontItemTemplate = this.aelFontInMarkup.shift(), this.aLIFontNames = jindo.$A(jindo.$$("LI", this.oDropdownLayer)).filter(function(e) {
                return null !== e.firstChild
            })._array, this.sDefaultText = this.elFontNameLabel.innerHTML
        },
        _initFontName: function() {
            var e;
            this._addFontInMarkup(), this.addAllFonts(), this.oApp.getCurrentStyle && (e = this.oApp.getCurrentStyle()) && this.$ON_MSG_STYLE_CHANGED("fontFamily", e.fontFamily), this.oApp.registerBrowserEvent(this.oDropdownLayer, "mouseover", "EVENT_FONTNAME_LAYER_MOUSEOVER", []), this.oApp.registerBrowserEvent(this.oDropdownLayer, "click", "EVENT_FONTNAME_LAYER_CLICKED", [])
        },
        _checkFontLI: function(e, t) {
            if (!e) return !1;
            var i = window.IsInstalledFont(t);
            return e.style.display = i ? "block" : "none", i
        },
        _addFontInMarkup: function() {
            for (var e, t, i, n, o = 0; e = this.aelFontInMarkup[o]; o++) e.firstChild ? (t = this._getFontFamilyFromLI(e).replace(this._rxQuote, "").replace(this._rxComma, ","), n |= this._checkFontLI(e, t)) : -1 < e.className.indexOf(this.FONT_SEPARATOR) ? (i && (i.style.display = n ? "block" : "none"), i = e, n = !1) : e.style.display = "none";
            i && (i.style.display = n ? "block" : "none")
        },
        _attachIEEvent: function() {
            if (this.htBrowser.ie) {
                if (this.htBrowser.nativeVersion < 9) return this._wfOnPasteWYSIWYGBody = jindo.$Fn(this._onPasteWYSIWYGBody, this), void this._wfOnPasteWYSIWYGBody.attach(this.oApp.getWYSIWYGDocument().body, "paste");
                if (document.documentMode < 9) return this._wfOnFocusWYSIWYGBody = jindo.$Fn(this._onFocusWYSIWYGBody, this), void this._wfOnFocusWYSIWYGBody.attach(this.oApp.getWYSIWYGDocument().body, "focus");
                this.welEditingAreaCover = jindo.$Element('<DIV style="width:100%; height:100%; position:absolute; top:0px; left:0px; z-index:1000;"></DIV>'), this.oApp.welEditingAreaContainer.prepend(this.welEditingAreaCover), jindo.$Fn(this._onMouseupCover, this).attach(this.welEditingAreaCover.$value(), "mouseup")
            }
        },
        _onFocusWYSIWYGBody: function() {
            this._wfOnFocusWYSIWYGBody.detach(this.oApp.getWYSIWYGDocument().body, "focus"), this._loadAllBaseFont()
        },
        _onPasteWYSIWYGBody: function() {
            this._wfOnPasteWYSIWYGBody.detach(this.oApp.getWYSIWYGDocument().body, "paste"), this._loadAllBaseFont()
        },
        _onMouseupCover: function(e) {
            e.stop(), this.welEditingAreaCover && this.welEditingAreaCover.leave();
            var t = e.mouse(),
                i = this.oApp.getWYSIWYGDocument().body,
                n = jindo.$Element(i),
                o = this.oApp.getEmptySelection();
            if (o.selectNode(i), o.collapseToStart(), o.select(), n.fireEvent("mousedown", {
                    left: t.left,
                    middle: t.middle,
                    right: t.right
                }), n.fireEvent("mouseup", {
                    left: t.left,
                    middle: t.middle,
                    right: t.right
                }), this.oApp.oNavigator.ie && document.documentMode < 11 && "WYSIWYG" === this.oApp.getEditingMode()) "<p></p>" == this.oApp.getWYSIWYGDocument().body.innerHTML && (this.oApp.getWYSIWYGDocument().body.innerHTML = '<p><span id="husky_bookmark_start_INIT"></span><span id="husky_bookmark_end_INIT"></span></p>', (o = this.oApp.getSelection()).moveToStringBookmark("INIT"), o.select(), o.removeStringBookmark("INIT"));
            else if (this.oApp.oNavigator.ie && 11 == this.oApp.oNavigator.nativeVersion && 11 == document.documentMode && "WYSIWYG" === this.oApp.getEditingMode() && "<p><br></p>" == this.oApp.getWYSIWYGDocument().body.innerHTML) {
                var s = jindo.$$.getSingle("br", i);
                o.setStartBefore(s), o.setEndBefore(s), o.select()
            }
        },
        $ON_EVENT_TOOLBAR_MOUSEDOWN: function() {
            this.htBrowser.nativeVersion < 9 || document.documentMode < 9 || this.welEditingAreaCover && this.welEditingAreaCover.leave()
        },
        _loadAllBaseFont: function() {
            var e, t;
            if (this.htBrowser.ie) {
                if (this.htBrowser.nativeVersion < 9)
                    for (e = 0, t = this.aBaseFontList.length; e < t; e++) this.aBaseFontList[e].loadCSS(this.oApp.getWYSIWYGDocument());
                else if (document.documentMode < 9)
                    for (e = 0, t = this.aBaseFontList.length; e < t; e++) this.aBaseFontList[e].loadCSSToMenu();
                this._loadAllBaseFont = function() {}
            }
        },
        _addFontToMenu: function(e, t, i) {
            var n = document.createElement("LI");
            n.innerHTML = this.elFontItemTemplate.innerHTML.replace("@DisplayName@", e).replace("FontFamily", t).replace("@SampleText@", i), this.elFontNameList.insertBefore(n, this.elFontItemTemplate), this.aLIFontNames[this.aLIFontNames.length] = n, 20 < this.aLIFontNames.length && (this.oDropdownLayer.style.overflowX = "hidden", this.oDropdownLayer.style.overflowY = "auto", this.oDropdownLayer.style.height = "400px", this.oDropdownLayer.style.width = "204px")
        },
        $ON_EVENT_FONTNAME_LAYER_MOUSEOVER: function(e) {
            var t = this._findLI(e.element);
            t && (this._clearLastHover(), t.className = "hover", this.elLastHover = t)
        },
        $ON_EVENT_FONTNAME_LAYER_CLICKED: function(e) {
            var t = this._findLI(e.element);
            if (t) {
                var i, n = this._getFontFamilyFromLI(t),
                    o = this.htAllFonts[n.replace(/"/g, nhn.husky.SE2M_FontNameWithLayerUI.CUSTOM_FONT_MARKS)];
                i = o ? o.defaultSize + "pt" : 0, this.oApp.exec("SET_FONTFAMILY", [n, i])
            }
        },
        _findLI: function(e) {
            for (;
                "LI" != e.tagName;) {
                if (!e || e === this.oDropdownLayer) return null;
                e = e.parentNode
            }
            return -1 < e.className.indexOf(this.FONT_SEPARATOR) ? null : e
        },
        _clearLastHover: function() {
            this.elLastHover && (this.elLastHover.className = "")
        },
        $ON_SE2M_TOGGLE_FONTNAME_LAYER: function() {
            this.oApp.exec("TOGGLE_TOOLBAR_ACTIVE_LAYER", [this.oDropdownLayer, null, "MSG_FONTNAME_LAYER_OPENED", [], "MSG_FONTNAME_LAYER_CLOSED", []]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["font"])
        },
        $ON_MSG_FONTNAME_LAYER_OPENED: function() {
            this.oApp.exec("SELECT_UI", ["fontName"])
        },
        $ON_MSG_FONTNAME_LAYER_CLOSED: function() {
            this._clearLastHover(), this.oApp.exec("DESELECT_UI", ["fontName"])
        },
        $ON_MSG_STYLE_CHANGED: function(e, t) {
            if ("fontFamily" == e) {
                t = t.replace(/["']/g, "");
                var i = this._getMatchingLI(t);
                if (this._clearFontNameSelection(), i) this.elFontNameLabel.innerHTML = this._getFontNameLabelFromLI(i), jindo.$Element(i).addClass("active");
                else {
                    var n = this.sDefaultText;
                    this.elFontNameLabel.innerHTML = n
                }
            }
        },
        $BEFORE_RECORD_UNDO_BEFORE_ACTION: function() {
            return !this.bDoNotRecordUndo
        },
        $BEFORE_RECORD_UNDO_AFTER_ACTION: function() {
            return !this.bDoNotRecordUndo
        },
        $BEFORE_RECORD_UNDO_ACTION: function() {
            return !this.bDoNotRecordUndo
        },
        $ON_SET_FONTFAMILY: function(e, t) {
            if (e) {
                var i = this.htAllFonts[e.replace(/"/g, nhn.husky.SE2M_FontNameWithLayerUI.CUSTOM_FONT_MARKS)];
                i && i.loadCSS(this.oApp.getWYSIWYGDocument()), this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["SET FONTFAMILY", {
                    bMustBlockElement: !0
                }]), this.bDoNotRecordUndo = !0, 0 < parseInt(t, 10) && this.oApp.exec("SET_WYSIWYG_STYLE", [{
                    fontSize: t
                }]), this.oApp.exec("SET_WYSIWYG_STYLE", [{
                    fontFamily: e
                }]), this.bDoNotRecordUndo = !1, this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["SET FONTFAMILY", {
                    bMustBlockElement: !0
                }]), this.oApp.exec("HIDE_ACTIVE_LAYER", []), this.oApp.exec("CHECK_STYLE_CHANGE", [])
            }
        },
        _getMatchingLI: function(e) {
            var t, i;
            e = e.toLowerCase();
            for (var n = 0; n < this.aLIFontNames.length; n++) {
                t = this.aLIFontNames[n], i = this._getFontFamilyFromLI(t).toLowerCase().split(",");
                for (var o = 0; o < i.length; o++)
                    if (i[o] && jindo.$S(i[o].replace(/['"]/gi, "")).trim().$value() == e) return t
            }
            return null
        },
        _getFontFamilyFromLI: function(e) {
            return e.getElementsByTagName("EM")[0].style.fontFamily
        },
        _getFontNameLabelFromLI: function(e) {
            return e.firstChild.firstChild.firstChild.nodeValue
        },
        _clearFontNameSelection: function() {
            for (var e = 0; e < this.aLIFontNames.length; e++) jindo.$Element(this.aLIFontNames[e]).removeClass("active")
        },
        addFont: function(e, t, i, n, o, s, r, a) {
            if (!this.htBrowser.ie && o) return null;
            if (a && !window.IsInstalledFont(e)) return null;
            var l, h, d = new c(e, t, i, n, o);
            return h = 0 < i ? (l = e + "_" + i, t + "_" + i) : (l = e, t), s || (l = nhn.husky.SE2M_FontNameWithLayerUI.CUSTOM_FONT_MARKS + l + nhn.husky.SE2M_FontNameWithLayerUI.CUSTOM_FONT_MARKS), this.htAllFonts[l] ? this.htAllFonts[l] : (this.htAllFonts[l] = d, this.htBrowser.ie && 9 <= this.htBrowser.nativeVersion && 9 <= document.documentMode && d.loadCSSToMenu(), this.htFamilyName2DisplayName[l] = t, r = r || this.oApp.$MSG("SE2M_FontNameWithLayerUI.sSampleText"), this._addFontToMenu(h, l, r), s ? 1 == s ? this.aDefaultFontList[this.aDefaultFontList.length] = d : this.aTempSavedFontList[this.aTempSavedFontList.length] = d : this.aBaseFontList[this.aBaseFontList.length] = d, d)
        },
        addFontInUse: function(e, t, i, n, o, s) {
            var r = this.addFont(e, t, i, n, o, s);
            return r ? (r.loadCSS(this.oApp.getWYSIWYGDocument()), r) : null
        },
        setMainFont: function(e, t, i, n, o, s) {
            var r = this.addFontInUse(e, t, i, n, o, s);
            return r ? (this.setDefaultFont(r.fontFamily, i), r) : null
        },
        setDefaultFont: function(e, t) {
            var i = this.oApp.getWYSIWYGDocument().body;
            i.style.fontFamily = e, 0 < t && (i.style.fontSize = t + "pt")
        }
    }), nhn.husky.SE2M_FontNameWithLayerUI.CUSTOM_FONT_MARKS = "'"
}, function(e, t) {
    nhn.ColorPicker = jindo.$Class({
        elem: null,
        huePanel: null,
        canvasType: "Canvas",
        _hsvColor: null,
        $init: function(e, t) {
            if (this.elem = jindo.$Element(e).empty(), this.huePanel = null, this.cursor = jindo.$Element("<div>").css("overflow", "hidden"), this.canvasType = jindo.$(e).filters ? "Filter" : jindo.$("<canvas>").getContext ? "Canvas" : null, !this.canvasType) return !1;
            for (var i in this.option({
                    huePanel: null,
                    huePanelType: "horizontal"
                }), this.option(t), this.option("huePanel") && (this.huePanel = jindo.$Element(this.option("huePanel")).empty()), this._hsvColor = this._hsv(0, 100, 100), this) /^_on[A-Z][a-z]+[A-Z][a-z]+$/.test(i) && (this[i + "Fn"] = jindo.$Fn(this[i], this));
            this._onDownColorFn.attach(this.elem, "mousedown"), this.huePanel && this._onDownHueFn.attach(this.huePanel, "mousedown"), this.paint()
        },
        rgb: function(e) {
            this.hsv(this._rgb2hsv(e.r, e.g, e.b))
        },
        hsv: function(e) {
            if (void 0 === e) return this._hsvColor;
            var t = null,
                i = this.elem.width(),
                n = this.elem.height(),
                o = this.cursor.width(),
                s = this.cursor.height(),
                r = 0,
                a = 0;
            if (this.huePanel) t = this._hsv2rgb(e.h, 100, 100), this.elem.css("background", "#" + this._rgb2hex(t.r, t.g, t.b)), r = e.s / 100 * i, a = (100 - e.v) / 100 * n;
            else {
                var l = i / 2;
                r = e.v > e.s ? (e.v = 100, e.s / 100 * l) : ((e.s = 100) - e.v) / 100 * l + l, a = e.h / 360 * n
            }
            r = Math.max(Math.min(r - 1, i - o), 1), a = Math.max(Math.min(a - 1, n - s), 1), this.cursor.css({
                left: r + "px",
                top: a + "px"
            }), this._hsvColor = e, t = this._hsv2rgb(e.h, e.s, e.v), this.fireEvent("colorchange", {
                type: "colorchange",
                element: this,
                currentElement: this,
                rgbColor: t,
                hexColor: "#" + this._rgb2hex(t.r, t.g, t.b),
                hsvColor: e
            })
        },
        paint: function() {
            this.huePanel ? (this["_paintColWith" + this.canvasType](), this["_paintHueWith" + this.canvasType]()) : this["_paintOneWith" + this.canvasType](), this.cursor.appendTo(this.elem), this.cursor.css({
                position: "absolute",
                top: "1px",
                left: "1px",
                background: "white",
                border: "1px solid black"
            }).width(3).height(3), this.hsv(this._hsvColor)
        },
        _paintColWithFilter: function() {
            jindo.$Element("<div>").css({
                position: "absolute",
                top: 0,
                left: 0,
                width: "100%",
                height: "100%",
                filter: "progid:DXImageTransform.Microsoft.Gradient(GradientType=1,StartColorStr='#FFFFFFFF',EndColorStr='#00FFFFFF')"
            }).appendTo(this.elem), jindo.$Element("<div>").css({
                position: "absolute",
                top: 0,
                left: 0,
                width: "100%",
                height: "100%",
                filter: "progid:DXImageTransform.Microsoft.Gradient(GradientType=0,StartColorStr='#00000000',EndColorStr='#FF000000')"
            }).appendTo(this.elem)
        },
        _paintColWithCanvas: function() {
            var e = jindo.$Element("<canvas>").css({
                width: "100%",
                height: "100%"
            });
            e.appendTo(this.elem.empty());
            var t = e.attr("width", e.width()).attr("height", e.height()).$value().getContext("2d"),
                i = null,
                n = e.width(),
                o = e.height();
            (i = t.createLinearGradient(0, 0, n, 0)).addColorStop(0, "rgba(255,255,255,1)"), i.addColorStop(1, "rgba(255,255,255,0)"), t.fillStyle = i, t.fillRect(0, 0, n, o), (i = t.createLinearGradient(0, 0, 0, o)).addColorStop(0, "rgba(0,0,0,0)"), i.addColorStop(1, "rgba(0,0,0,1)"), t.fillStyle = i, t.fillRect(0, 0, n, o)
        },
        _paintOneWithFilter: function() {
            for (var e, t, i, n, o, s, r = this.elem.height(), a = 1; a < 7; a++) e = Math.floor((a - 1) / 6 * r), t = Math.floor(a / 6 * r), i = this._hsv2rgb((a - 1) / 6 * 360, 100, 100), n = this._hsv2rgb(a / 6 * 360, 100, 100), o = "#FF" + this._rgb2hex(i.r, i.g, i.b), s = "#FF" + this._rgb2hex(n.r, n.g, n.b), jindo.$Element("<div>").css({
                position: "absolute",
                left: 0,
                width: "100%",
                top: e + "px",
                height: t - e + "px",
                filter: "progid:DXImageTransform.Microsoft.Gradient(GradientType=0,StartColorStr='" + o + "',EndColorStr='" + s + "')"
            }).appendTo(this.elem);
            jindo.$Element("<div>").css({
                position: "absolute",
                top: 0,
                left: 0,
                width: "50%",
                height: "100%",
                filter: "progid:DXImageTransform.Microsoft.Gradient(GradientType=1,StartColorStr='#FFFFFFFF',EndColorStr='#00FFFFFF')"
            }).appendTo(this.elem), jindo.$Element("<div>").css({
                position: "absolute",
                top: 0,
                right: 0,
                width: "50%",
                height: "100%",
                filter: "progid:DXImageTransform.Microsoft.Gradient(GradientType=1,StartColorStr='#00000000',EndColorStr='#FF000000')"
            }).appendTo(this.elem)
        },
        _paintOneWithCanvas: function() {
            var e = {
                    r: 0,
                    g: 0,
                    b: 0
                },
                t = jindo.$Element("<canvas>").css({
                    width: "100%",
                    height: "100%"
                });
            t.appendTo(this.elem.empty());
            for (var i = t.attr("width", t.width()).attr("height", t.height()).$value().getContext("2d"), n = t.width(), o = t.height(), s = i.createLinearGradient(0, 0, 0, o), r = 0; r < 7; r++) e = this._hsv2rgb(r / 6 * 360, 100, 100), s.addColorStop(r / 6, "rgb(" + e.join(",") + ")");
            i.fillStyle = s, i.fillRect(0, 0, n, o), (s = i.createLinearGradient(0, 0, n, 0)).addColorStop(0, "rgba(255,255,255,1)"), s.addColorStop(.5, "rgba(255,255,255,0)"), s.addColorStop(.5, "rgba(0,0,0,0)"), s.addColorStop(1, "rgba(0,0,0,1)"), i.fillStyle = s, i.fillRect(0, 0, n, o)
        },
        _paintHueWithFilter: function() {
            var e, t, i, n, o, s, r = "vertical" == this.option().huePanelType,
                a = this.huePanel.width(),
                l = this.huePanel.height(),
                h = null,
                d = parseInt(this.huePanel.css("borderWidth"), 10);
            isNaN(d) && (d = 0), a -= 2 * d;
            for (var c = 1; c < 7; c++) {
                e = Math.floor((c - 1) / 6 * (r ? l : a)), t = Math.floor(c / 6 * (r ? l : a)), i = this._hsv2rgb((c - 1) / 6 * 360, 100, 100), n = this._hsv2rgb(c / 6 * 360, 100, 100), o = "#FF" + this._rgb2hex(i.r, i.g, i.b), s = "#FF" + this._rgb2hex(n.r, n.g, n.b);
                var p = t - e + 1;
                (h = jindo.$Element("<div>").css({
                    position: "absolute",
                    filter: "progid:DXImageTransform.Microsoft.Gradient(GradientType=" + (r ? 0 : 1) + ",StartColorStr='" + o + "',EndColorStr='" + s + "')"
                })).appendTo(this.huePanel), h.css(r ? "left" : "top", 0).css(r ? "width" : "height", "100%"), h.css(r ? "top" : "left", e + "px").css(r ? "height" : "width", p + "px")
            }
        },
        _paintHueWithCanvas: function() {
            var e, t = "vertical" == this.option().huePanelType,
                i = jindo.$Element("<canvas>").css({
                    width: "100%",
                    height: "100%"
                });
            i.appendTo(this.huePanel.empty());
            for (var n = i.attr("width", i.width()).attr("height", i.height()).$value().getContext("2d"), o = n.createLinearGradient(0, 0, t ? 0 : i.width(), t ? i.height() : 0), s = 0; s < 7; s++) e = this._hsv2rgb(s / 6 * 360, 100, 100), o.addColorStop(s / 6, "rgb(" + e.join(",") + ")");
            n.fillStyle = o, n.fillRect(0, 0, i.width(), i.height())
        },
        _rgb2hsv: function(e, t, i) {
            var n = 0,
                o = 0,
                s = Math.max(e, t, i),
                r = s - Math.min(e, t, i);
            return (o = s ? r / s : 0) && (e == s ? n = 60 * (t - i) / r : t == s ? n = 120 + 60 * (i - e) / r : i == s && (n = 240 + 60 * (e - t) / r), n < 0 && (n += 360)), n = Math.floor(n), o = Math.floor(100 * o), s = Math.floor(s / 255 * 100), this._hsv(n, o, s)
        },
        _hsv2rgb: function(e, t, i) {
            e = e % 360 / 60, t /= 100, i /= 100;
            var n = 0,
                o = 0,
                s = 0,
                r = Math.floor(e),
                a = e - r,
                l = i * (1 - t),
                h = i * (1 - t * a),
                d = i * (1 - t * (1 - a));
            switch (r) {
                case 0:
                    n = i, o = d, s = l;
                    break;
                case 1:
                    n = h, o = i, s = l;
                    break;
                case 2:
                    n = l, o = i, s = d;
                    break;
                case 3:
                    n = l, o = h, s = i;
                    break;
                case 4:
                    n = d, o = l, s = i;
                    break;
                case 5:
                    n = i, o = l, s = h
            }
            return n = Math.floor(255 * n), o = Math.floor(255 * o), s = Math.floor(255 * s), this._rgb(n, o, s)
        },
        _rgb2hex: function(e, t, i) {
            return 1 == (e = e.toString(16)).length && (e = "0" + e), 1 == (t = t.toString(16)).length && (t = "0" + t), 1 == (i = i.toString(16)).length && (i = "0" + i), e + t + i
        },
        _hex2rgb: function(e) {
            var t = e.match(/#?([0-9a-f]{6}|[0-9a-f]{3})/i);
            return t = 3 == t[1].length ? t[1].match(/./g).filter(function(e) {
                return e + e
            }) : t[1].match(/../g), {
                r: Number("0x" + t[0]),
                g: Number("0x" + t[1]),
                b: Number("0x" + t[2])
            }
        },
        _rgb: function(e, t, i) {
            var n = [e, t, i];
            return n.r = e, n.g = t, n.b = i, n
        },
        _hsv: function(e, t, i) {
            var n = [e, t, i];
            return n.h = e, n.s = t, n.v = i, n
        },
        _onDownColor: function(e) {
            if (!e.mouse().left) return !1;
            var t = e.pos();
            this._colPagePos = [t.pageX, t.pageY], this._colLayerPos = [t.layerX, t.layerY], this._onUpColorFn.attach(document, "mouseup"), this._onMoveColorFn.attach(document, "mousemove"), this._onMoveColor(e)
        },
        _onUpColor: function() {
            this._onUpColorFn.detach(document, "mouseup"), this._onMoveColorFn.detach(document, "mousemove")
        },
        _onMoveColor: function(e) {
            var t = this._hsvColor,
                i = e.pos(),
                n = this._colLayerPos[0] + (i.pageX - this._colPagePos[0]),
                o = this._colLayerPos[1] + (i.pageY - this._colPagePos[1]),
                s = this.elem.width(),
                r = this.elem.height();
            if (n = Math.max(Math.min(n, s), 0), o = Math.max(Math.min(o, r), 0), this.huePanel) t.s = t[1] = n / s * 100, t.v = t[2] = (r - o) / r * 100;
            else {
                t.h = o / r * 360;
                var a = s / 2;
                n < a ? (t.s = n / a * 100, t.v = 100) : (t.s = 100, t.v = (s - n) / a * 100)
            }
            this.hsv(t), e.stop()
        },
        _onDownHue: function(e) {
            if (!e.mouse().left) return !1;
            var t = e.pos();
            this._huePagePos = [t.pageX, t.pageY], this._hueLayerPos = [t.layerX, t.layerY], this._onUpHueFn.attach(document, "mouseup"), this._onMoveHueFn.attach(document, "mousemove"), this._onMoveHue(e)
        },
        _onUpHue: function() {
            this._onUpHueFn.detach(document, "mouseup"), this._onMoveHueFn.detach(document, "mousemove")
        },
        _onMoveHue: function(e) {
            var t = this._hsvColor,
                i = e.pos(),
                n = 0,
                o = 0,
                s = this._hueLayerPos[0] + (i.pageX - this._huePagePos[0]),
                r = this._hueLayerPos[1] + (i.pageY - this._huePagePos[1]);
            o = "vertical" == this.option().huePanelType ? (n = r, this.huePanel.height()) : (n = s, this.huePanel.width()), t.h = t[0] = Math.min(Math.max(n, 0), o) / o * 360 % 360, this.hsv(t), e.stop()
        }
    }).extend(jindo.Component)
}, function(e, t) {
    nhn.husky.SE2M_Accessibility = jindo.$Class({
        name: "SE2M_Accessibility",
        $init: function(e, t, i) {
            this._assignHTMLElements(e), t && (this.sLang = t), i && (this.sType = i)
        },
        _assignHTMLElements: function(e) {
            this.elHelpPopupLayer = jindo.$$.getSingle("DIV.se2_accessibility", e), this.welHelpPopupLayer = jindo.$Element(this.elHelpPopupLayer), this.oCloseButton = jindo.$$.getSingle("BUTTON.se2_close", this.elHelpPopupLayer), this.oCloseButton2 = jindo.$$.getSingle("BUTTON.se2_close2", this.elHelpPopupLayer), this.nDefaultTop = 150, this.elAppContainer = e
        },
        $ON_MSG_APP_READY: function() {
            this.htAccessOption = nhn.husky.SE2M_Configuration.SE2M_Accessibility || {}, this.oApp.exec("REGISTER_HOTKEY", ["alt+F10", "FOCUS_TOOLBAR_AREA", []]), this.oApp.exec("REGISTER_HOTKEY", ["alt+COMMA", "FOCUS_BEFORE_ELEMENT", []]), this.oApp.exec("REGISTER_HOTKEY", ["alt+PERIOD", "FOCUS_NEXT_ELEMENT", []]), this.sLang && "ko_KR" !== this.sLang || (this.oApp.exec("REGISTER_HOTKEY", ["alt+0", "OPEN_HELP_POPUP", []]), this.oApp.exec("REGISTER_HOTKEY", ["esc", "CLOSE_HELP_POPUP", [], document]), this.htAccessOption.sTitleElementId && this.oApp.registerBrowserEvent(document.getElementById(this.htAccessOption.sTitleElementId), "keydown", "MOVE_TO_EDITAREA", []))
        },
        $ON_MOVE_TO_EDITAREA: function(e) {
            if (9 == e.key().keyCode) {
                if (e.key().shift) return;
                this.oApp.delayedExec("FOCUS", [], 0)
            }
        },
        $LOCAL_BEFORE_FIRST: function() {
            jindo.$Fn(jindo.$Fn(this.oApp.exec, this.oApp).bind("CLOSE_HELP_POPUP", [this.oCloseButton]), this).attach(this.oCloseButton, "click"), jindo.$Fn(jindo.$Fn(this.oApp.exec, this.oApp).bind("CLOSE_HELP_POPUP", [this.oCloseButton2]), this).attach(this.oCloseButton2, "click");
            var e = this.oApp.getWYSIWYGWindow().frameElement;
            this.htOffsetPos = jindo.$Element(e).offset(), this.nEditorWidth = e.offsetWidth, this.htInitialPos = this.welHelpPopupLayer.offset(), this.nLayerWidth = 590, this.nLayerHeight = 480, this.htTopLeftCorner = {
                x: parseInt(this.htOffsetPos.left, 10),
                y: parseInt(this.htOffsetPos.top, 10)
            }
        },
        $ON_FOCUS_NEXT_ELEMENT: function() {
            this._currentNextFocusElement = null, this.htAccessOption.sNextElementId ? this._currentNextFocusElement = document.getElementById(this.htAccessOption.sNextElementId) : this._currentNextFocusElement = this._findNextFocusElement(this.elAppContainer), this._currentNextFocusElement ? (window.focus(), this._currentNextFocusElement.focus()) : parent && parent.nhn && parent.nhn.husky && parent.nhn.husky.EZCreator && parent.nhn.husky.EZCreator.elIFrame && (parent.focus(), (this._currentNextFocusElement = this._findNextFocusElement(parent.nhn.husky.EZCreator.elIFrame)) && this._currentNextFocusElement.focus())
        },
        _findNextFocusElement: function(e) {
            for (var t = null, i = e.nextSibling; i;) {
                if (1 !== i.nodeType) {
                    if (i = this._switchToSiblingOrNothing(i)) continue;
                    break
                }
                if (this._recursivePreorderTraversalFilter(i, this._isFocusTag), this._nextFocusElement) {
                    t = this._nextFocusElement, this._bStopFindingNextElement = !1, this._nextFocusElement = null;
                    break
                }
                if (!(i = this._switchToSiblingOrNothing(i))) break
            }
            return t
        },
        _switchToSiblingOrNothing: function(e, t) {
            var i = e;
            if (t)
                if (i.previousSibling) i = i.previousSibling;
                else {
                    for (;
                        "BODY" != i.nodeName.toUpperCase() && !i.previousSibling;) i = i.parentNode;
                    i = "BODY" == i.nodeName.toUpperCase() ? null : i.previousSibling
                }
            else if (i.nextSibling) i = i.nextSibling;
            else {
                for (;
                    "BODY" != i.nodeName.toUpperCase() && !i.nextSibling;) i = i.parentNode;
                i = "BODY" == i.nodeName.toUpperCase() ? null : i.nextSibling
            }
            return i
        },
        _recursivePreorderTraversalFilter: function(e, t, i) {
            var n = this;
            if (t.apply(e)) return n._bStopFindingNextElement = !0, void(i ? n._previousFocusElement = e : n._nextFocusElement = e);
            if (i)
                for (var o = e.childNodes.length, s = o - 1; 0 <= s && (n._recursivePreorderTraversalFilter(e.childNodes[s], t, !0), !this._bStopFindingNextElement); s--);
            else
                for (s = 0, o = e.childNodes.length; s < o && (n._recursivePreorderTraversalFilter(e.childNodes[s], t), !this._bStopFindingNextElement); s++);
        },
        _isFocusTag: function() {
            for (var e = this, t = ["A", "BUTTON", "INPUT", "TEXTAREA"], i = !1, n = 0, o = t.length; n < o; n++)
                if (1 === e.nodeType && e.nodeName && e.nodeName.toUpperCase() == t[n] && !e.disabled && jindo.$Element(e).visible()) {
                    i = !0;
                    break
                } return i
        },
        $ON_FOCUS_BEFORE_ELEMENT: function() {
            this._currentPreviousFocusElement = null, this.htAccessOption.sBeforeElementId ? this._currentPreviousFocusElement = document.getElementById(this.htAccessOption.sBeforeElementId) : this._currentPreviousFocusElement = this._findPreviousFocusElement(this.elAppContainer), this._currentPreviousFocusElement ? (window.focus(), this._currentPreviousFocusElement.focus()) : parent && parent.nhn && parent.nhn.husky && parent.nhn.husky.EZCreator && parent.nhn.husky.EZCreator.elIFrame && (parent.focus(), (this._currentPreviousFocusElement = this._findPreviousFocusElement(parent.nhn.husky.EZCreator.elIFrame)) && this._currentPreviousFocusElement.focus())
        },
        _findPreviousFocusElement: function(e) {
            for (var t = null, i = e.previousSibling; i;) {
                if (1 !== i.nodeType) {
                    if (i = this._switchToSiblingOrNothing(i, !0)) continue;
                    break
                }
                if (this._recursivePreorderTraversalFilter(i, this._isFocusTag, !0), this._previousFocusElement) {
                    t = this._previousFocusElement, this._bStopFindingNextElement = !1, this._previousFocusElement = null;
                    break
                }
                if (!(i = this._switchToSiblingOrNothing(i, !0))) break
            }
            return t
        },
        $ON_FOCUS_TOOLBAR_AREA: function() {
            this.oButton = jindo.$$.getSingle("BUTTON.se2_font_family", this.elAppContainer), this.oButton && !this.oButton.disabled && (window.focus(), this.oButton.focus())
        },
        $ON_OPEN_HELP_POPUP: function() {
            this.oApp.exec("DISABLE_ALL_UI", [{
                aExceptions: ["se2_accessibility"]
            }]), this.oApp.exec("SHOW_EDITING_AREA_COVER"), this.oApp.exec("SELECT_UI", ["se2_accessibility"]), this.nCalcX = this.htTopLeftCorner.x + this.oApp.getEditingAreaWidth() - this.nLayerWidth, this.nCalcY = this.htTopLeftCorner.y - 30, this.oApp.exec("SHOW_DIALOG_LAYER", [this.elHelpPopupLayer, {
                elHandle: this.elTitle,
                nMinX: this.htTopLeftCorner.x + 0,
                nMinY: this.nDefaultTop + 77,
                nMaxX: this.nCalcX,
                nMaxY: this.nCalcY
            }]), this.welHelpPopupLayer.offset(this.nCalcY, this.nCalcX / 2), jindo.$Agent().navigator().ie && window.focus();
            var t = this;
            setTimeout(function() {
                try {
                    t.oCloseButton2.focus()
                } catch (e) {}
            }, 200)
        },
        $ON_CLOSE_HELP_POPUP: function() {
            this.oApp.exec("ENABLE_ALL_UI"), this.oApp.exec("DESELECT_UI", ["helpPopup"]), this.oApp.exec("HIDE_ALL_DIALOG_LAYER", []), this.oApp.exec("HIDE_EDITING_AREA_COVER"), this.oApp.exec("FOCUS")
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_SCharacter = jindo.$Class({
        name: "SE2M_SCharacter",
        $ON_MSG_APP_READY: function() {
            this.oApp.exec("REGISTER_UI_EVENT", ["sCharacter", "click", "TOGGLE_SCHARACTER_LAYER"]), this.oApp.registerLazyMessage(["TOGGLE_SCHARACTER_LAYER"], ["hp_SE2M_SCharacter$Lazy.js"])
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_FindReplacePlugin = jindo.$Class({
        name: "SE2M_FindReplacePlugin",
        oEditingWindow: null,
        oFindReplace: null,
        bFindMode: !0,
        bLayerShown: !1,
        $init: function() {
            this.nDefaultTop = 20
        },
        $ON_MSG_APP_READY: function() {
            this.oEditingWindow = this.oApp.getWYSIWYGWindow(), jindo.$Agent().os().mac ? (this.oApp.exec("REGISTER_HOTKEY", ["meta+f", "SHOW_FIND_LAYER", []]), this.oApp.exec("REGISTER_HOTKEY", ["meta+h", "SHOW_REPLACE_LAYER", []])) : (this.oApp.exec("REGISTER_HOTKEY", ["ctrl+f", "SHOW_FIND_LAYER", []]), this.oApp.exec("REGISTER_HOTKEY", ["ctrl+h", "SHOW_REPLACE_LAYER", []])), this.oApp.exec("REGISTER_UI_EVENT", ["findAndReplace", "click", "TOGGLE_FIND_REPLACE_LAYER"]), this.oApp.registerLazyMessage(["TOGGLE_FIND_REPLACE_LAYER", "SHOW_FIND_LAYER", "SHOW_REPLACE_LAYER", "SHOW_FIND_REPLACE_LAYER"], ["hp_SE2M_FindReplacePlugin$Lazy.js", "N_FindReplace.js"])
        },
        $ON_SHOW_ACTIVE_LAYER: function() {
            this.oApp.exec("HIDE_DIALOG_LAYER", [this.elDropdownLayer])
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_Quote = jindo.$Class({
        name: "SE2M_Quote",
        htQuoteStyles_view: null,
        $init: function() {
            var e = nhn.husky.SE2M_Configuration.Quote || {},
                t = e.sImageBaseURL;
            this.nMaxLevel = e.nMaxLevel || 14, this.htQuoteStyles_view = {}, this.htQuoteStyles_view.se2_quote1 = "_zoom:1;padding:0 8px; margin:0 0 30px 20px; margin-right:15px; border-left:2px solid #cccccc;color:#888888;", this.htQuoteStyles_view.se2_quote2 = "_zoom:1;margin:0 0 30px 13px;padding:0 8px 0 16px;background:url(" + t + "/bg_quote2.gif) 0 3px no-repeat;color:#888888;", this.htQuoteStyles_view.se2_quote3 = "_zoom:1;margin:0 0 30px 0;padding:10px;border:1px dashed #cccccc;color:#888888;", this.htQuoteStyles_view.se2_quote4 = "_zoom:1;margin:0 0 30px 0;padding:10px;border:1px dashed #66b246;color:#888888;", this.htQuoteStyles_view.se2_quote5 = "_zoom:1;margin:0 0 30px 0;padding:10px;border:1px dashed #cccccc;background:url(" + t + "/bg_b1.png) repeat;_background:none;_filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(src='" + t + "/bg_b1.png',sizingMethod='scale');color:#888888;", this.htQuoteStyles_view.se2_quote6 = "_zoom:1;margin:0 0 30px 0;padding:10px;border:1px solid #e5e5e5;color:#888888;", this.htQuoteStyles_view.se2_quote7 = "_zoom:1;margin:0 0 30px 0;padding:10px;border:1px solid #66b246;color:#888888;", this.htQuoteStyles_view.se2_quote8 = "_zoom:1;margin:0 0 30px 0;padding:10px;border:1px solid #e5e5e5;background:url(" + t + "/bg_b1.png) repeat;_background:none;_filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(src='" + t + "/bg_b1.png',sizingMethod='scale');color:#888888;", this.htQuoteStyles_view.se2_quote9 = "_zoom:1;margin:0 0 30px 0;padding:10px;border:2px solid #e5e5e5;color:#888888;", this.htQuoteStyles_view.se2_quote10 = "_zoom:1;margin:0 0 30px 0;padding:10px;border:2px solid #e5e5e5;background:url(" + t + "/bg_b1.png) repeat;_background:none;_filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(src='" + t + "/bg_b1.png',sizingMethod='scale');color:#888888;"
        },
        _assignHTMLElements: function() {
            this.elDropdownLayer = jindo.$$.getSingle("DIV.husky_seditor_blockquote_layer", this.oApp.htOptions.elAppContainer), this.aLI = jindo.$$("LI", this.elDropdownLayer)
        },
        $ON_REGISTER_CONVERTERS: function() {
            this.oApp.exec("ADD_CONVERTER", ["DB_TO_IR", jindo.$Fn(function(e) {
                return e = e.replace(/<(blockquote)[^>]*class=['"]?(se2_quote[0-9]+)['"]?[^>]*>/gi, "<$1 class=$2>")
            }, this).bind()]), this.oApp.exec("ADD_CONVERTER", ["IR_TO_DB", jindo.$Fn(function(e) {
                var n = this.htQuoteStyles_view;
                return e = e.replace(/<(blockquote)[^>]*class=['"]?(se2_quote[0-9]+)['"]?[^>]*>/gi, function(e, t, i) {
                    return "<" + t + " class=" + i + ' style="' + n[i] + '">'
                })
            }, this).bind()]), this.htSE1toSE2Map = {
                "01": "1",
                "02": "2",
                "03": "6",
                "04": "8",
                "05": "9",
                "07": "3",
                "08": "5"
            }
        },
        $LOCAL_BEFORE_FIRST: function() {
            this._assignHTMLElements(), this.oApp.registerBrowserEvent(this.elDropdownLayer, "click", "EVENT_SE2_BLOCKQUOTE_LAYER_CLICK", []), this.oApp.delayedExec("SE2_ATTACH_HOVER_EVENTS", [this.aLI], 0)
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.exec("REGISTER_UI_EVENT", ["quote", "click", "TOGGLE_BLOCKQUOTE_LAYER"]), this.oApp.registerLazyMessage(["TOGGLE_BLOCKQUOTE_LAYER"], ["hp_SE2M_Quote$Lazy.js"])
        },
        $ON_EVENT_EDITING_AREA_KEYDOWN: function(e) {
            var t, i;
            "WYSIWYG" === this.oApp.getEditingMode() && 8 === e.key().keyCode && ((t = this.oApp.getSelection()).fixCommonAncestorContainer(), (i = this._findParentQuote(t.commonAncestorContainer)) && this._isBlankQuote(i) && (e.stop(jindo.$Event.CANCEL_DEFAULT), t.selectNode(i), t.collapseToStart(), jindo.$Element(i).leave(), t.select()))
        },
        $ON_EVENT_EDITING_AREA_KEYUP: function(e) {
            var t, i, n;
            if ("WYSIWYG" === this.oApp.getEditingMode() && 46 === e.key().keyCode) {
                if ((t = this.oApp.getSelection()).fixCommonAncestorContainer(), !(i = this._findParentQuote(t.commonAncestorContainer))) return !1;
                i.nextSibling || (e.stop(jindo.$Event.CANCEL_DEFAULT), (n = t._document.createElement("P")).innerHTML = "&nbsp;", jindo.$Element(i).after(n), setTimeout(jindo.$Fn(function(e) {
                    var t = e.placeStringBookmark();
                    e.select(), e.removeStringBookmark(t)
                }, this).bind(t), 0))
            }
        },
        _isBlankQuote: function(e) {
            function t(e) {
                return "" === (e = e.replace(/[\r\n]/gi, "").replace(unescape("%uFEFF"), "")) || ("&nbsp;" === e || " " === e)
            }
            var i, n, o, s, r, a, l = this.oApp.oNavigator.chrome,
                h = this.oApp.oNavigator.safari;
            if (t(e.innerHTML) || "<br>" === e.innerHTML) return !0;
            if (l || h) {
                var d, c = jindo.$$("TABLE", e),
                    p = c.length;
                for (o = 0; o < p; o++) d = c[o], r = d, 0 === jindo.$$("tr", r).length && jindo.$Element(d).leave()
            }
            for (o = 0, s = (n = e.childNodes).length; o < s; o++)
                if (i = n[o], (3 !== (a = i).nodeType || !t(a.nodeValue)) && ("P" !== a.tagName && "SPAN" !== a.tagName || !t(a.innerHTML) && "<br>" !== a.innerHTML)) return !1;
            return !0
        },
        _findParentQuote: function(e) {
            return this._findAncestor(jindo.$Fn(function(e) {
                if (!e) return !1;
                if ("BLOCKQUOTE" !== e.tagName) return !1;
                if (!e.className) return !1;
                var t = e.className;
                return !!this.htQuoteStyles_view[t]
            }, this).bind(), e)
        },
        _findAncestor: function(e, t) {
            for (; t && !e(t);) t = t.parentNode;
            return t
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_TableCreator = jindo.$Class({
        name: "SE2M_TableCreator",
        _sSETblClass: "__se_tbl",
        nRows: 3,
        nColumns: 4,
        nBorderSize: 1,
        sBorderColor: "#000000",
        sBGColor: "#000000",
        nBorderStyleIdx: 3,
        nTableStyleIdx: 1,
        nMinRows: 1,
        nMaxRows: 20,
        nMinColumns: 1,
        nMaxColumns: 20,
        nMinBorderWidth: 1,
        nMaxBorderWidth: 10,
        rxLastDigits: null,
        sReEditGuideMsg_table: null,
        oSelection: null,
        $ON_MSG_APP_READY: function() {
            this.sReEditGuideMsg_table = this.oApp.$MSG("SE2M_ReEditAction.reEditGuideMsg.table"), this.oApp.exec("REGISTER_UI_EVENT", ["table", "click", "TOGGLE_TABLE_LAYER"]), this.oApp.registerLazyMessage(["TOGGLE_TABLE_LAYER"], ["hp_SE2M_TableCreator$Lazy.js"])
        },
        $ON_REGISTER_CONVERTERS: function() {
            this.oApp.exec("ADD_CONVERTER_DOM", ["IR_TO_DB", jindo.$Fn(this.irToDbDOM, this).bind()]), this.oApp.exec("ADD_CONVERTER_DOM", ["DB_TO_IR", jindo.$Fn(this.dbToIrDOM, this).bind()])
        },
        irToDbDOM: function(e) {
            var t = [],
                i = jindo.$$("table[class=__se_tbl]", e, {
                    oneTimeOffCache: !0
                });
            if (jindo.$A(i).forEach(function(e) {
                    jindo.$Element(e).attr("attr_no_border_tbl") && t.push(e)
                }, this), !(t.length < 1))
                for (var n, o = [], s = 0, r = t.length; s < r; s++) n = t[s], jindo.$Element(n).css({
                    border: "",
                    borderLeft: "",
                    borderBottom: ""
                }), jindo.$Element(n).attr({
                    border: 0,
                    cellpadding: 1
                }), o = jindo.$$("tbody>tr>td", n), jindo.$A(o).forEach(function(e) {
                    jindo.$Element(e).css({
                        border: "",
                        borderTop: "",
                        borderRight: ""
                    })
                })
        },
        dbToIrDOM: function(e) {
            var t = [],
                i = jindo.$$("table[class=__se_tbl]", e, {
                    oneTimeOffCache: !0
                });
            if (jindo.$A(i).forEach(function(e) {
                    jindo.$Element(e).attr("attr_no_border_tbl") && t.push(e)
                }, this), !(t.length < 1))
                for (var n, o = [], s = 0, r = t.length; s < r; s++) n = t[s], jindo.$Element(n).css({
                    border: "1px dashed #c7c7c7",
                    borderLeft: 0,
                    borderBottom: 0
                }), jindo.$Element(n).attr({
                    border: 1,
                    cellpadding: 0
                }), o = jindo.$$("tbody>tr>td", n), jindo.$A(o).forEach(function(e) {
                    jindo.$Element(e).css({
                        border: "1px dashed #c7c7c7",
                        borderTop: 0,
                        borderRight: 0
                    })
                })
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_TableBlockStyler = jindo.$Class({
        name: "SE2M_TableBlockStyler",
        nSelectedTD: 0,
        htSelectedTD: {},
        aTdRange: [],
        $init: function() {},
        $LOCAL_BEFORE_ALL: function() {
            return "WYSIWYG" == this.oApp.getEditingMode()
        },
        $ON_MSG_APP_READY: function() {
            this.oDocument = this.oApp.getWYSIWYGDocument()
        },
        $ON_EVENT_EDITING_AREA_MOUSEUP: function() {
            "WYSIWYG" == this.oApp.getEditingMode() && this.setTdBlock()
        },
        $ON_IS_SELECTED_TD_BLOCK: function(e, t) {
            return 0 < this.nSelectedTD ? t[e] = !0 : t[e] = !1, t[e]
        },
        $ON_GET_SELECTED_TD_BLOCK: function(e, t) {
            t[e] = this.htSelectedTD.aTdCells
        },
        setTdBlock: function() {
            this.oApp.exec("GET_SELECTED_CELLS", ["aTdCells", this.htSelectedTD]);
            var e = this.htSelectedTD.aTdCells;
            e && (this.nSelectedTD = e.length)
        },
        $ON_DELETE_BLOCK_CONTENTS: function() {
            var t, i, n, o = this;
            this.setTdBlock();
            for (var e = 0; e < this.nSelectedTD; e++) jindo.$Element(this.htSelectedTD.aTdCells[e]).child(function(e) {
                (t = jindo.$Element(e._element.parentNode)).remove(e), i = o.oDocument.createElement("P"), n = jindo.$Agent().navigator().firefox ? o.oDocument.createElement("BR") : o.oDocument.createTextNode(" "), i.appendChild(n), t.append(i)
            }, 1)
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_StyleRemover = jindo.$Class({
        name: "SE2M_StyleRemover",
        $ON_MSG_APP_READY: function() {
            this.oApp.exec("REGISTER_UI_EVENT", ["styleRemover", "click", "CHOOSE_REMOVE_STYLE", []])
        },
        $LOCAL_BEFORE_FIRST: function() {
            this.oHuskyRange = this.oApp.getEmptySelection(), this._document = this.oHuskyRange._document
        },
        $ON_CHOOSE_REMOVE_STYLE: function() {
            var e, t = {};
            this.oApp.exec("IS_SELECTED_TD_BLOCK", ["bIsSelectedTd", t]), e = t.bIsSelectedTd, this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["REMOVE STYLE", {
                bMustBlockElement: !0
            }]), e ? this.oApp.exec("REMOVE_STYLE_IN_BLOCK", []) : this.oApp.exec("REMOVE_STYLE", []), this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["REMOVE STYLE", {
                bMustBlockElement: !0
            }]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["noeffect"])
        },
        $ON_REMOVE_STYLE_IN_BLOCK: function() {
            var e = {};
            this.oSelection = this.oApp.getSelection(), this.oApp.exec("GET_SELECTED_TD_BLOCK", ["aTdCells", e]);
            for (var t = e.aTdCells, i = 0; i < t.length; i++) this.oSelection.selectNodeContents(t[i]), this.oSelection.select(), this.oApp.exec("REMOVE_STYLE", [])
        },
        $ON_REMOVE_STYLE: function(e) {
            if (e && e.commonAncestorContainer || (e = this.oApp.getSelection()), !e.collapsed) {
                e.expandBothEnds();
                var t, i, n, o = e.placeStringBookmark(),
                    s = e.getNodes(!0);
                this._removeStyle(s), e.moveToBookmark(o), s = e.getNodes(!0);
                for (var r = 0; r < s.length; r++) {
                    var a = s[r];
                    a.style && "BR" != a.tagName && "TD" != a.tagName && "TR" != a.tagName && "TBODY" != a.tagName && "TABLE" != a.tagName && (a.removeAttribute("align"), a.removeAttribute("style"), "inline" != jindo.$Element(a).css("display") || "IMG" == a.tagName || "IFRAME" == a.tagName || a.firstChild && !e._isBlankTextNode(a.firstChild) || a.parentNode.removeChild(a))
                }
                for (e.moveToBookmark(o), "TBODY" === e.commonAncestorContainer.tagName && (e = this.oApp.getSelection()), e.select(), t = this._document.getElementById(e.HUSKY_BOOMARK_START_ID_PREFIX + o); t;) {
                    for ((i = nhn.DOMFix.parentNode(t)).removeChild(t); i && (!i.firstChild || !i.firstChild.nextSibling && e._isBlankTextNode(i.firstChild));) n = i.parentNode, i.parentNode.removeChild(i), i = n;
                    t = this._document.getElementById(e.HUSKY_BOOMARK_START_ID_PREFIX + o)
                }
                for (t = this._document.getElementById(e.HUSKY_BOOMARK_END_ID_PREFIX + o); t;) {
                    for ((i = nhn.DOMFix.parentNode(t)).removeChild(t); i && (!i.firstChild || !i.firstChild.nextSibling && e._isBlankTextNode(i.firstChild));) n = i.parentNode, i.parentNode.removeChild(i), i = n;
                    t = this._document.getElementById(e.HUSKY_BOOMARK_END_ID_PREFIX + o)
                }
                this.oApp.exec("CHECK_STYLE_CHANGE")
            }
        },
        $ON_REMOVE_STYLE2: function(e) {
            this._removeStyle(e)
        },
        $ON_REMOVE_STYLE_AND_PASTE_HTML: function(e, t) {
            var i, n, o, s, r;
            if (i = jindo.$Agent().navigator(), !e) return !1;
            "WYSIWYG" != this.oApp.getEditingMode() && this.oApp.exec("CHANGE_EDITING_MODE", ["WYSIWYG"]), t || this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["REMOVE STYLE AND PASTE HTML"]), (r = this.oApp.getSelection()).deleteContents(), n = this.oApp.getWYSIWYGDocument().createElement("DIV"), r.insertNode(n), i.webkit && (n.innerHTML = "&nbsp;"), r.selectNode(n), this.oApp.exec("REMOVE_STYLE", [r]), i.ie ? e += "<p>&nbsp;</p>" : i.firefox && (e += "<p>\ufeff<br></p>"), r.selectNode(n), r.pasteHTML(e), s = r.getNodes() || [];
            for (var a = 0; a < s.length; a++)
                if (s[a].tagName && "td" == s[a].tagName.toLowerCase()) {
                    o = s[a], r.selectNodeContents(o.firstChild || o), r.collapseToStart(), r.select();
                    break
                } r.collapseToEnd(), r.select(), this.oApp.exec("FOCUS"), n || n.parentNode.removeChild(n), t || this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["REMOVE STYLE AND PASTE HTML"])
        },
        _removeStyle: function(e) {
            for (var t = jindo.$A(e), i = 0; i < e.length; i++) {
                var n = e[i];
                if (n && n.parentNode && n.parentNode.tagName) {
                    if ("inline" == jindo.$Element(n.parentNode).css("display")) {
                        var o = n.parentNode;
                        if (n.firstChild) {
                            if (-1 == t.indexOf(this.oHuskyRange._getVeryLastRealChild(n))) continue;
                            if (-1 == t.indexOf(this.oHuskyRange._getVeryFirstRealChild(n))) continue
                        }
                        if (n.nextSibling) {
                            if (-1 == t.indexOf(this._getVeryLast(n.nextSibling)))
                                if (i--, -1 != t.indexOf(this._getVeryFirst(n.parentNode))) {
                                    a = n;
                                    for (var s = o; a && (l = a.previousSibling, o.parentNode.insertBefore(a, s), s = a, l);) a = l;
                                    0 === o.childNodes.length && o.parentNode.removeChild(o)
                                } else {
                                    !1;
                                    var r = this._document.createElement("SPAN");
                                    for (a = n, o.insertBefore(r, a.nextSibling); a && (l = a.previousSibling, r.insertBefore(a, r.firstChild), l) && -1 != t.indexOf(this._getVeryFirst(l));) a = l;
                                    for (this._splitAndAppendAtTop(r); r.firstChild;) r.parentNode.insertBefore(r.firstChild, r);
                                    r.parentNode.removeChild(r)
                                }
                        } else {
                            i--;
                            for (var a = n; a;) {
                                var l = a.previousSibling;
                                if (o.parentNode.insertBefore(a, o.nextSibling), !l) break;
                                if (-1 == t.indexOf(this._getVeryFirst(l))) break;
                                a = l
                            }
                            0 === o.childNodes.length && o.parentNode.removeChild(o)
                        }
                    }
                }
            }
        },
        _splitAndAppendAtTop: function(e) {
            for (var t = e, i = t, n = i;
                "inline" == jindo.$Element(i.parentNode).css("display");) {
                for (var o = i.parentNode.cloneNode(!1); i.nextSibling;) o.appendChild(i.nextSibling);
                i = i.parentNode, o.insertBefore(n, o.firstChild), n = o
            }
            var s = i.parentNode;
            s.insertBefore(t, i.nextSibling), s.insertBefore(n, t.nextSibling)
        },
        _getVeryFirst: function(e) {
            return e ? e.firstChild ? this.oHuskyRange._getVeryFirstRealChild(e) : e : null
        },
        _getVeryLast: function(e) {
            return e ? e.lastChild ? this.oHuskyRange._getVeryLastRealChild(e) : e : null
        }
    })
}, function(e, t) {
    nhn.husky.SE2M_TableEditor = jindo.$Class({
        name: "SE2M_TableEditor",
        _sSETblClass: "__se_tbl",
        _sSEReviewTblClass: "__se_tbl_review",
        STATUS: {
            S_0: 1,
            MOUSEDOWN_CELL: 2,
            CELL_SELECTING: 3,
            CELL_SELECTED: 4,
            MOUSEOVER_BORDER: 5,
            MOUSEDOWN_BORDER: 6
        },
        CELL_SELECTION_CLASS: "se2_te_selection",
        MIN_CELL_WIDTH: 5,
        MIN_CELL_HEIGHT: 5,
        TMP_BGC_ATTR: "_se2_tmp_te_bgc",
        TMP_BGIMG_ATTR: "_se2_tmp_te_bg_img",
        ATTR_TBL_TEMPLATE: "_se2_tbl_template",
        nStatus: 1,
        nMouseEventsStatus: 0,
        aSelectedCells: [],
        $ON_REGISTER_CONVERTERS: function() {
            this.oApp.exec("ADD_CONVERTER_DOM", ["WYSIWYG_TO_IR", jindo.$Fn(function(e) {
                if (!(this.aSelectedCells.length < 1))
                    for (var t, i = ["TD", "TH"], n = 0; n < i.length; n++)
                        for (var o = 0, s = (t = e.getElementsByTagName(i[n])).length; o < s; o++) t[o].className && (t[o].className = t[o].className.replace(this.CELL_SELECTION_CLASS, ""), t[o].getAttribute(this.TMP_BGC_ATTR) ? (t[o].style.backgroundColor = t[o].getAttribute(this.TMP_BGC_ATTR), t[o].removeAttribute(this.TMP_BGC_ATTR)) : t[o].getAttribute(this.TMP_BGIMG_ATTR) && (jindo.$Element(this.aCells[o]).css("backgroundImage", t[o].getAttribute(this.TMP_BGIMG_ATTR)), t[o].removeAttribute(this.TMP_BGIMG_ATTR)))
            }, this).bind()])
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.registerLazyMessage(["EVENT_EDITING_AREA_MOUSEMOVE", "STYLE_TABLE"], ["hp_SE2M_TableEditor$Lazy.js", "SE2M_TableTemplate.js"])
        }
    })
}, function(module, exports) {
    nhn.husky.SE2M_QuickEditor_Common = jindo.$Class({
        name: "SE2M_QuickEditor_Common",
        _environmentData: "",
        _currentType: "",
        _in_event: !1,
        _bUseConfig: !1,
        _sBaseAjaxUrl: "",
        _sAddTextAjaxUrl: "",
        $init: function() {
            this.waHotkeys = new jindo.$A([]), this.waHotkeyLayers = new jindo.$A([])
        },
        $ON_MSG_APP_READY: function() {
            var e = nhn.husky.SE2M_Configuration.QuickEditor;
            e && (this._bUseConfig = !e.common || "undefined" == typeof e.common.bUseConfig || e.common.bUseConfig), this._bUseConfig ? (this._sBaseAjaxUrl = e.common.sBaseAjaxUrl, this._sAddTextAjaxUrl = e.common.sAddTextAjaxUrl, this.getData()) : this.setData("{table:'full',img:'full',review:'full'}"), this.oApp.registerLazyMessage(["OPEN_QE_LAYER"], ["hp_SE2M_QuickEditor_Common$Lazy.js"])
        },
        $ON_EVENT_EDITING_AREA_KEYDOWN: function(e) {
            var t = e.key();
            if (8 == t.keyCode || 46 == t.keyCode) {
                var i = jindo.$Agent().navigator();
                if (i.ie && 8 < i.nativeVersion) {
                    var n = jindo.$$.getSingle("DIV.husky_seditor_editing_area_container").childNodes[0];
                    "DIV" == n.tagName && 1e3 == n.style.zIndex && n.parentNode.removeChild(n)
                }
                this.oApp.exec("CLOSE_QE_LAYER", [e])
            }
        },
        getData: function() {
            var i = this;
            jindo.$Ajax(i._sBaseAjaxUrl, {
                type: "jsonp",
                timeout: 1,
                onload: function(e) {
                    var t = e.json().result;
                    t && t.text_data ? i.setData(t.text_data) : i.setData("{table:'full',img:'full',review:'full'}")
                },
                onerror: function() {
                    i.setData("{table:'full',img:'full',review:'full'}")
                },
                ontimeout: function() {
                    i.setData("{table:'full',img:'full',review:'full'}")
                }
            }).request({
                text_key: "qeditor_fold"
            })
        },
        setData: function(sResult) {
            var oResult = {
                table: "full",
                img: "full",
                review: "full"
            };
            sResult && (oResult = eval("(" + sResult + ")")), this._environmentData = {
                table: {
                    isOpen: !1,
                    type: oResult.table,
                    isFixed: !1,
                    position: []
                },
                img: {
                    isOpen: !1,
                    type: oResult.img,
                    isFixed: !1
                },
                review: {
                    isOpen: !1,
                    type: oResult.review,
                    isFixed: !1,
                    position: []
                }
            }, this.waTableTagNames = jindo.$A(["table", "tbody", "td", "tfoot", "th", "thead", "tr"])
        },
        $ON_REGISTER_HOTKEY: function(e, t, i) {
            "tab" != e && "shift+tab" != e && this.waHotkeys.push([e, t, i])
        }
    })
}, function(e, t) {
    function a(e, t) {
        e = e.replace(/\s+/g, "");
        var i = a.Store,
            n = a.Action;
        if (void 0 === t && e.constructor == String) return i.set("document", e, document), n.init(i.get("document"), e);
        if (t.constructor == String && e.constructor == String) return i.set(t, e, jindo.$(t)), n.init(i.get(t), e);
        if (t.constructor != String && e.constructor == String) {
            var o = "nonID" + (new Date).getTime();
            return o = a.Store.searchId(o, t), i.set(o, e, t), n.init(i.get(o), e)
        }
        alert(t + "는 반드시 string이거나  없어야 됩니다.")
    }
    a.Store = {
            anthorKeyHash: {},
            datas: {},
            currentId: "",
            currentKey: "",
            searchId: function(i, n) {
                var o = !1;
                if (jindo.$H(this.datas).forEach(function(e, t) {
                        n == e.element && (i = t, o = !0, jindo.$H.Break())
                    }), !o && i in this.datas) {
                    for (var e = i; e in this.datas;) e = "nonID" + (new Date).getTime();
                    return e
                }
                return i
            },
            set: function(e, t, i) {
                this.currentId = e, this.currentKey = t;
                var n = this.get(e);
                this.datas[e] = n ? n.createKey(t) : new a.Data(e, t, i)
            },
            get: function(e, t) {
                return t ? this.datas[e].keys[t] : this.datas[e]
            },
            reset: function(e) {
                var t = this.datas[e];
                a.Helper.bind(t.func, t.element, "detach"), delete this.datas[e]
            },
            allReset: function() {
                jindo.$H(this.datas).forEach(jindo.$Fn(function(e, t) {
                    this.reset(t)
                }, this).bind())
            }
        }, a.Data = jindo.$Class({
            $init: function(e, t, i) {
                this.id = e, this.element = i, this.func = jindo.$Fn(this.fire, this).bind(), a.Helper.bind(this.func, i, "attach"), this.keys = {}, this.keyStemp = {}, this.createKey(t)
            },
            createKey: function(e) {
                this.keyStemp[a.Helper.keyInterpretor(e)] = e, this.keys[e] = {};
                var t = this.keys[e];
                return t.key = e, t.events = [], t.commonExceptions = [], t.stopDefalutBehavior = !0, this
            },
            getKeyStamp: function(e) {
                var t = e.keyCode || e.charCode,
                    i = "";
                return i += e.altKey ? "1" : "0", i += e.ctrlKey ? "1" : "0", i += e.metaKey ? "1" : "0", i += e.shiftKey ? "1" : "0", i += t
            },
            fire: function(e) {
                e = e || window.eEvent;
                var t = this.keyStemp[this.getKeyStamp(e)];
                t && this.excute(new jindo.$Event(e), t)
            },
            excute: function(o, e) {
                var s = !0,
                    t = a.Helper,
                    r = this.keys[e];
                t.notCommonException(o, r.commonExceptions) && jindo.$A(r.events).forEach(function(e) {
                    var t;
                    if (r.stopDefalutBehavior) {
                        var i = e.exceptions.length;
                        if (i) {
                            for (var n = 0; n < i; n++)
                                if (!e.exception[n](o)) {
                                    s = !1;
                                    break
                                } s ? (e.event(o), jindo.$Agent().navigator().ie && ((t = o._event).keyCode = "", t.charCode = ""), o.stop()) : jindo.$A.Break()
                        } else e.event(o), jindo.$Agent().navigator().ie && ((t = o._event).keyCode = "", t.charCode = ""), o.stop()
                    }
                })
            },
            addEvent: function(e, t) {
                var i = this.keys[t].events;
                a.Helper.hasEvent(e, i) || i.push({
                    event: e,
                    exceptions: []
                })
            },
            addException: function(e, t) {
                var i = this.keys[t].commonExceptions;
                a.Helper.hasException(e, i) || i.push(e)
            },
            removeException: function(t, e) {
                var i = this.keys[e].commonExceptions;
                jindo.$A(i).filter(function(e) {
                    return e != t
                }).$value()
            },
            removeEvent: function(t, e) {
                var i = this.keys[e].events;
                jindo.$A(i).filter(function(e) {
                    return e != t
                }).$value(), this.unRegister(e)
            },
            unRegister: function(e) {
                this.keys[e].events.length && delete this.keys[e];
                var t = !0;
                for (var i in this.keys) {
                    t = !1;
                    break
                }
                t && (a.Helper.bind(this.func, this.element, "detach"), delete a.Store.datas[this.id])
            },
            startDefalutBehavior: function(e) {
                this._setDefalutBehavior(e, !1)
            },
            stopDefalutBehavior: function(e) {
                this._setDefalutBehavior(e, !0)
            },
            _setDefalutBehavior: function(e, t) {
                this.keys[e].stopDefalutBehavior = t
            }
        }), a.Helper = {
            keyInterpretor: function(e) {
                var t = e.split("+"),
                    i = jindo.$A(t),
                    n = "";
                n += i.has("alt") ? "1" : "0", n += i.has("ctrl") ? "1" : "0", n += i.has("meta") ? "1" : "0", n += i.has("shift") ? "1" : "0";
                var o = (i = i.filter(function(e) {
                    return !("alt" == e || "ctrl" == e || "meta" == e || "shift" == e)
                })).$value()[0];
                return o && (n += e = a.Store.anthorKeyHash[o.toUpperCase()] || o.toUpperCase().charCodeAt(0)), n
            },
            notCommonException: function(e, t) {
                for (var i = t.length, n = 0; n < i; n++)
                    if (!t[n](e)) return !1;
                return !0
            },
            hasEvent: function(e, t) {
                for (var i = t.length, n = 0; n < i; ++n)
                    if (t.event == e) return !0;
                return !1
            },
            hasException: function(e, t) {
                for (var i = t.length, n = 0; n < i; ++n)
                    if (t[n] == e) return !0;
                return !1
            },
            bind: function(e, t, i) {
                "attach" == i ? window.domAttach(t, "keydown", e) : window.domDetach(t, "keydown", e)
            }
        }, document.addEventListener ? window.domAttach = function(e, t, i) {
            e.addEventListener(t, i, !1)
        } : window.domAttach = function(e, t, i) {
            e.attachEvent("on" + t, i)
        }, document.removeEventListener ? window.domDetach = function(e, t, i) {
            e.removeEventListener(t, i, !1)
        } : window.domDetach = function(e, t, i) {
            e.detachEvent("on" + t, i)
        }, a.Action = {
            init: function(e, t) {
                return this.dataInstance = e, this.rawKey = t, this
            },
            addEvent: function(e) {
                return this.dataInstance.addEvent(e, this.rawKey), this
            },
            removeEvent: function(e) {
                return this.dataInstance.removeEvent(e, this.rawKey), this
            },
            addException: function(e) {
                return this.dataInstance.addException(e, this.rawKey), this
            },
            removeException: function(e) {
                return this.dataInstance.removeException(e, this.rawKey), this
            },
            startDefalutBehavior: function() {
                return this.dataInstance.startDefalutBehavior(this.rawKey), this
            },
            stopDefalutBehavior: function() {
                return this.dataInstance.stopDefalutBehavior(this.rawKey), this
            },
            resetElement: function() {
                return a.Store.reset(this.dataInstance.id), this
            },
            resetAll: function() {
                return a.Store.allReset(), this
            }
        },
        function() {
            a.Store.anthorKeyHash = {
                BACKSPACE: 8,
                TAB: 9,
                ENTER: 13,
                ESC: 27,
                SPACE: 32,
                PAGEUP: 33,
                PAGEDOWN: 34,
                END: 35,
                HOME: 36,
                LEFT: 37,
                UP: 38,
                RIGHT: 39,
                DOWN: 40,
                DEL: 46,
                COMMA: 188,
                PERIOD: 190,
                SLASH: 191
            };
            for (var e = a.Store.anthorKeyHash, t = 1; t < 13; t++) a.Store.anthorKeyHash["F" + t] = t + 111;
            var i = jindo.$Agent().navigator();
            i.ie || i.safari || i.chrome ? (e.HYPHEN = 189, e.EQUAL = 187) : (e.HYPHEN = 109, e.EQUAL = 61)
        }(), window.shortcut = a
}, function(e, t) {
    nhn.husky.Hotkey = jindo.$Class({
        name: "Hotkey",
        $init: function() {
            this.oShortcut = window.shortcut
        },
        $ON_ADD_HOTKEY: function(e, t, i, n) {
            i = i || [];
            var o = jindo.$Fn(this.oApp.exec, this.oApp).bind(t, i);
            this.oShortcut(e, n).addEvent(o)
        }
    })
}, function(e, t) {
    nhn.husky.SE_UndoRedo = jindo.$Class({
        name: "SE_UndoRedo",
        oCurStateIdx: null,
        iMinimumSizeChange: 1,
        nMaxUndoCount: 20,
        nAfterMaxDeleteBuffer: 100,
        sBlankContentsForFF: "<br>",
        sDefaultXPath: "/HTML[0]/BODY[0]",
        $init: function() {
            this.aUndoHistory = [], this.oCurStateIdx = {
                nIdx: 0,
                nStep: 0
            }, this.nHardLimit = this.nMaxUndoCount + this.nAfterMaxDeleteBuffer
        },
        $LOCAL_BEFORE_ALL: function(e) {
            if (e.match(/_DO_RECORD_UNDO_HISTORY_AT$/)) return !0;
            try {
                if ("WYSIWYG" != this.oApp.getEditingMode()) return !1
            } catch (t) {
                return !1
            }
            return !0
        },
        $BEFORE_MSG_APP_READY: function() {
            this._historyLength = 0, this.oApp.exec("ADD_APP_PROPERTY", ["getUndoHistory", jindo.$Fn(this._getUndoHistory, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["getUndoStateIdx", jindo.$Fn(this._getUndoStateIdx, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["saveSnapShot", jindo.$Fn(this._saveSnapShot, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["getLastKey", jindo.$Fn(this._getLastKey, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["setLastKey", jindo.$Fn(this._setLastKey, this).bind()]), this._saveSnapShot(), this.oApp.exec("DO_RECORD_UNDO_HISTORY_AT", [this.oCurStateIdx, "", "", "", null, this.sDefaultXPath])
        },
        _getLastKey: function() {
            return this.sLastKey
        },
        _setLastKey: function(e) {
            this.sLastKey = e
        },
        $ON_MSG_APP_READY: function() {
            var e = jindo.$Agent().navigator();
            this.bIE = e.ie, this.bFF = e.firefox, this.oApp.exec("REGISTER_UI_EVENT", ["undo", "click", "UNDO"]), this.oApp.exec("REGISTER_UI_EVENT", ["redo", "click", "REDO"]), jindo.$Agent().os().mac ? (this.oApp.exec("REGISTER_HOTKEY", ["meta+z", "UNDO"]), this.oApp.exec("REGISTER_HOTKEY", ["meta+y", "REDO"])) : (this.oApp.exec("REGISTER_HOTKEY", ["ctrl+z", "UNDO"]), this.oApp.exec("REGISTER_HOTKEY", ["ctrl+y", "REDO"]))
        },
        $ON_UNDO: function() {
            if (this._doRecordUndoHistory("UNDO", {
                    nStep: 0,
                    bSkipIfEqual: !0,
                    bMustBlockContainer: !0
                }), !(this.oCurStateIdx.nIdx <= 0)) {
                var e = this.aUndoHistory[this.oCurStateIdx.nIdx].oUndoCallback[this.oCurStateIdx.nStep],
                    t = this.aUndoHistory[this.oCurStateIdx.nIdx].sParentXPath[this.oCurStateIdx.nStep];
                if (e && this.oApp.exec(e.sMsg, e.aParams), 0 < this.oCurStateIdx.nStep) this.oCurStateIdx.nStep--;
                else {
                    var i = this.aUndoHistory[this.oCurStateIdx.nIdx];
                    this.oCurStateIdx.nIdx--, 1 < i.nTotalSteps ? this.oCurStateIdx.nStep = 0 : (i = this.aUndoHistory[this.oCurStateIdx.nIdx], this.oCurStateIdx.nStep = i.nTotalSteps - 1)
                }
                var n = this.aUndoHistory[this.oCurStateIdx.nIdx].sParentXPath[this.oCurStateIdx.nStep],
                    o = !1;
                n !== t && 0 === n.indexOf(t) && (o = !0), this.oApp.exec("RESTORE_UNDO_HISTORY", [this.oCurStateIdx.nIdx, this.oCurStateIdx.nStep, o]), this.oApp.exec("CHECK_STYLE_CHANGE", []), this.sLastKey = null
            }
        },
        $ON_REDO: function() {
            if (!(this.oCurStateIdx.nIdx >= this.aUndoHistory.length)) {
                var e = this.aUndoHistory[this.oCurStateIdx.nIdx];
                if (!(this.oCurStateIdx.nIdx == this.aUndoHistory.length - 1 && this.oCurStateIdx.nStep >= e.nTotalSteps - 1)) {
                    this.oCurStateIdx.nStep < e.nTotalSteps - 1 ? this.oCurStateIdx.nStep++ : (this.oCurStateIdx.nIdx++, e = this.aUndoHistory[this.oCurStateIdx.nIdx], this.oCurStateIdx.nStep = e.nTotalSteps - 1);
                    var t = this.aUndoHistory[this.oCurStateIdx.nIdx].oRedoCallback[this.oCurStateIdx.nStep];
                    t && this.oApp.exec(t.sMsg, t.aParams), this.oApp.exec("RESTORE_UNDO_HISTORY", [this.oCurStateIdx.nIdx, this.oCurStateIdx.nStep]), this.oApp.exec("CHECK_STYLE_CHANGE", []), this.sLastKey = null
                }
            }
        },
        $ON_RECORD_UNDO_ACTION: function(e, t) {
            (t = t || {
                sSaveTarget: null,
                elSaveTarget: null,
                bMustBlockElement: !1,
                bMustBlockContainer: !1,
                bDontSaveSelection: !1
            }).nStep = 0, t.bSkipIfEqual = !1, t.bTwoStepAction = !1, this._doRecordUndoHistory(e, t)
        },
        $ON_RECORD_UNDO_BEFORE_ACTION: function(e, t) {
            (t = t || {
                sSaveTarget: null,
                elSaveTarget: null,
                bMustBlockElement: !1,
                bMustBlockContainer: !1,
                bDontSaveSelection: !1
            }).nStep = 0, t.bSkipIfEqual = !1, t.bTwoStepAction = !0, this._doRecordUndoHistory(e, t)
        },
        $ON_RECORD_UNDO_AFTER_ACTION: function(e, t) {
            (t = t || {
                sSaveTarget: null,
                elSaveTarget: null,
                bMustBlockElement: !1,
                bMustBlockContainer: !1,
                bDontSaveSelection: !1
            }).nStep = 1, t.bSkipIfEqual = !1, t.bTwoStepAction = !0, this._doRecordUndoHistory(e, t)
        },
        $ON_RESTORE_UNDO_HISTORY: function(e, t, i) {
            this.oApp.exec("HIDE_ACTIVE_LAYER"), this.oCurStateIdx.nIdx = e, this.oCurStateIdx.nStep = t;
            var n = this.aUndoHistory[this.oCurStateIdx.nIdx],
                o = n.sContent[this.oCurStateIdx.nStep],
                s = n.sFullContents[this.oCurStateIdx.nStep],
                r = n.oBookmark[this.oCurStateIdx.nStep],
                a = n.sParentXPath[this.oCurStateIdx.nStep],
                l = null,
                h = "",
                d = this.oApp.getEmptySelection();
            if (this.oApp.exec("RESTORE_IE_SELECTION"), i) this.oApp.getWYSIWYGDocument().body.innerHTML = s, h = s = this.oApp.getWYSIWYGDocument().body.innerHTML, a = this.sDefaultXPath;
            else {
                l = d._evaluateXPath(a, d._document);
                try {
                    l.innerHTML = o, h = l.innerHTML
                } catch (c) {
                    this.oApp.getWYSIWYGDocument().body.innerHTML = s, h = s = this.oApp.getWYSIWYGDocument().body.innerHTML, a = this.sDefaultXPath
                }
            }
            this.bFF && h == this.sBlankContentsForFF && (h = ""), n.sFullContents[this.oCurStateIdx.nStep] = s, n.sContent[this.oCurStateIdx.nStep] = h, n.sParentXPath[this.oCurStateIdx.nStep] = a, r && "scroll" == r.sType ? setTimeout(jindo.$Fn(function() {
                this.oApp.getWYSIWYGDocument().documentElement.scrollTop = r.nScrollTop
            }, this).bind(), 0) : (d = this.oApp.getEmptySelection()).selectionLoaded && (r ? d.moveToXPathBookmark(r) : d = this.oApp.getEmptySelection(), d.select())
        },
        _doRecordUndoHistory: function(e, t) {
            var i = (t = t || {}).nStep || 0,
                n = t.bSkipIfEqual || !1,
                o = t.bTwoStepAction || !1,
                s = t.sSaveTarget || null,
                r = t.elSaveTarget || null,
                a = t.bDontSaveSelection || !1,
                l = t.bMustBlockElement || !1,
                h = t.bMustBlockContainer || !1,
                d = t.oUndoCallback,
                c = t.oRedoCallback;
            this._historyLength = this.aUndoHistory.length, this.oCurStateIdx.nIdx !== this._historyLength - 1 && (n = !0);
            var p = this.aUndoHistory[this.oCurStateIdx.nIdx],
                u = p.sFullContents[this.oCurStateIdx.nStep],
                _ = "",
                E = "",
                g = "",
                f = null,
                A = null,
                S = {
                    nIdx: this.oCurStateIdx.nIdx,
                    nStep: this.oCurStateIdx.nStep
                };
            if ((A = this.oApp.getSelection()).selectionLoaded && (f = A.getXPathBookmark()), g = r ? A._getXPath(r) : s ? this._getTargetXPath(f, s) : this._getParentXPath(f, l, h), E = this.oApp.getWYSIWYGDocument().body.innerHTML, _ = g === this.sDefaultXPath ? E : A._evaluateXPath(g, A._document).innerHTML, this.bFF && _ == this.sBlankContentsForFF && (_ = ""), !o && n) {
                if (u.length === E.length) return;
                var T = document.createElement("div"),
                    m = document.createElement("div");
                T.innerHTML = E, m.innerHTML = u;
                var C = document.createDocumentFragment();
                if (C.appendChild(T), C.appendChild(m), E = T.innerHTML, u = m.innerHTML, C = m = T = null, u.length === E.length) return
            }
            a && (f = {
                sType: "scroll",
                nScrollTop: this.oApp.getWYSIWYGDocument().documentElement.scrollTop
            }), S.nStep = i, 0 === S.nStep && this.oCurStateIdx.nStep === p.nTotalSteps - 1 && (S.nIdx = this.oCurStateIdx.nIdx + 1), this._doRecordUndoHistoryAt(S, e, _, E, f, g, d, c)
        },
        $ON_DO_RECORD_UNDO_HISTORY_AT: function(e, t, i, n, o, s) {
            this._doRecordUndoHistoryAt(e, t, i, n, o, s)
        },
        _doRecordUndoHistoryAt: function(e, t, i, n, o, s, r, a) {
            if (0 !== e.nStep) this.aUndoHistory[e.nIdx].nTotalSteps = e.nStep + 1, this.aUndoHistory[e.nIdx].sContent[e.nStep] = i, this.aUndoHistory[e.nIdx].sFullContents[e.nStep] = n, this.aUndoHistory[e.nIdx].oBookmark[e.nStep] = o, this.aUndoHistory[e.nIdx].sParentXPath[e.nStep] = s, this.aUndoHistory[e.nIdx].oUndoCallback[e.nStep] = r, this.aUndoHistory[e.nIdx].oRedoCallback[e.nStep] = a;
            else {
                var l = {
                    sAction: t,
                    nTotalSteps: 1,
                    sContent: []
                };
                l.sContent[0] = i, l.sFullContents = [], l.sFullContents[0] = n, l.oBookmark = [], l.oBookmark[0] = o, l.sParentXPath = [], l.sParentXPath[0] = s, l.oUndoCallback = [], l.oUndoCallback[0] = r, l.oRedoCallback = [], l.oRedoCallback[0] = a, this.aUndoHistory.splice(e.nIdx, this._historyLength - e.nIdx, l), this._historyLength = this.aUndoHistory.length
            }
            this._historyLength > this.nHardLimit && (this.aUndoHistory.splice(0, this.nAfterMaxDeleteBuffer), e.nIdx -= this.nAfterMaxDeleteBuffer), this.oCurStateIdx.nIdx = e.nIdx, this.oCurStateIdx.nStep = e.nStep
        },
        _saveSnapShot: function() {
            this.oSnapShot = {
                oBookmark: this.oApp.getSelection().getXPathBookmark()
            }
        },
        _getTargetXPath: function(e, t) {
            var i = this.sDefaultXPath,
                n = e[0].sXPath.split("/"),
                o = e[1].sXPath.split("/"),
                s = [],
                r = n.length < o.length ? n.length : o.length,
                a = 0,
                l = -1;
            if ("BODY" === t) return i;
            for (a = 0; a < r && n[a] === o[a]; a++) s.push(n[a]), "" !== n[a] && "HTML" !== n[a] && "BODY" !== n[a] && -1 < n[a].indexOf(t) && (l = a);
            return -1 < l && (s.length = l), (i = s.join("/")).length < this.sDefaultXPath.length && (i = this.sDefaultXPath), i
        },
        _getParentXPath: function(e, t, i) {
            var n, o, s, r, a, l, h, d, c = this.sDefaultXPath,
                p = ["", "HTML[0]", "BODY[0]"],
                u = 0,
                _ = -1;
            if (!e) return c;
            if (e[0].sXPath === c || e[1].sXPath === c) return c;
            if (n = e[0].sXPath.split("/"), o = e[1].sXPath.split("/"), s = this.oSnapShot.oBookmark[0].sXPath.split("/"), r = this.oSnapShot.oBookmark[1].sXPath.split("/"), a = s.length < r.length ? s.length : r.length, (l = (l = n.length < o.length ? n.length : o.length) < a ? l : a) < 3) return c;
            for (t = t || !1, i = i || !1, u = 3; u < l && ((h = n[u]) === o[u] && h === s[u] && h === r[u] && o[u] === s[u] && o[u] === r[u] && s[u] === r[u]); u++) p.push(h), d = h.substring(0, h.indexOf("[")), (!t || "P" !== d && "LI" !== d && "DIV" !== d) && "UL" !== d && "OL" !== d && "TD" !== d && "TR" !== d && "TABLE" !== d && "BLOCKQUOTE" !== d || (_ = u);
            if (-1 < _) p.length = _ + 1;
            else if (t || i) return c;
            return p.join("/")
        },
        _getUndoHistory: function() {
            return this.aUndoHistory
        },
        _getUndoStateIdx: function() {
            return this.oCurStateIdx
        }
    })
}, function(e, t) {
    nhn.husky.Utils = jindo.$Class({
        name: "Utils",
        $init: function() {
            var e = jindo.$Agent().navigator();
            if (e.ie && 6 == e.version) try {
                document.execCommand("BackgroundImageCache", !1, !0)
            } catch (t) {}
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["htBrowser", jindo.$Agent().navigator()])
        },
        $ON_ATTACH_HOVER_EVENTS: function(e, t) {
            var i = (t = t || []).sHoverClass || "hover",
                n = t.fnElmToSrc || function(e) {
                    return e
                },
                o = t.fnElmToTarget || function(e) {
                    return e
                };
            if (e)
                for (var s = jindo.$Fn(function(e) {
                        jindo.$Element(o(e.currentElement)).addClass(i)
                    }, this), r = jindo.$Fn(function(e) {
                        jindo.$Element(o(e.currentElement)).removeClass(i)
                    }, this), a = 0, l = e.length; a < l; a++) {
                    var h = n(e[a]);
                    s.attach(h, "mouseover"), r.attach(h, "mouseout"), s.attach(h, "focus"), r.attach(h, "blur")
                }
        }
    })
}, function(e, t) {
    nhn.husky.DialogLayerManager = jindo.$Class({
        name: "DialogLayerManager",
        aMadeDraggable: null,
        aOpenedLayers: null,
        $init: function() {
            this.aMadeDraggable = [], this.aDraggableLayer = [], this.aOpenedLayers = []
        },
        $ON_MSG_APP_READY: function() {
            this.oApp.registerLazyMessage(["SHOW_DIALOG_LAYER", "TOGGLE_DIALOG_LAYER"], ["hp_DialogLayerManager$Lazy.js", "N_DraggableLayer.js"])
        }
    })
}, function(e, t) {
    nhn.husky.ActiveLayerManager = jindo.$Class({
        name: "ActiveLayerManager",
        oCurrentLayer: null,
        $BEFORE_MSG_APP_READY: function() {
            this.oNavigator = jindo.$Agent().navigator()
        },
        $ON_TOGGLE_ACTIVE_LAYER: function(e, t, i, n, o) {
            e == this.oCurrentLayer ? this.oApp.exec("HIDE_ACTIVE_LAYER", []) : (this.oApp.exec("SHOW_ACTIVE_LAYER", [e, n, o]), t && this.oApp.exec(t, i))
        },
        $ON_SHOW_ACTIVE_LAYER: function(e, t, i) {
            (e = jindo.$(e)) != this.oCurrentLayer && (this.oApp.exec("HIDE_ACTIVE_LAYER", []), this.sOnCloseCmd = t, this.aOnCloseParam = i, e.style.display = "block", this.oCurrentLayer = e, this.oApp.exec("ADD_APP_PROPERTY", ["oToolBarLayer", this.oCurrentLayer]))
        },
        $ON_HIDE_ACTIVE_LAYER: function() {
            var e = this.oCurrentLayer;
            e && (e.style.display = "none", this.oCurrentLayer = null, this.sOnCloseCmd && this.oApp.exec(this.sOnCloseCmd, this.aOnCloseParam))
        },
        $ON_HIDE_ACTIVE_LAYER_IF_NOT_CHILD: function(e) {
            for (var t = e; t;) {
                if (t == this.oCurrentLayer) return;
                t = t.parentNode
            }
            this.oApp.exec("HIDE_ACTIVE_LAYER")
        },
        $ON_HIDE_CURRENT_ACTIVE_LAYER: function() {
            this.oApp.exec("HIDE_ACTIVE_LAYER", [])
        }
    })
}, function(e, t) {
    nhn.husky.StringConverterManager = jindo.$Class({
        name: "StringConverterManager",
        oConverters: null,
        $init: function() {
            this.oConverters = {}, this.oConverters_DOM = {}, this.oAgent = jindo.$Agent().navigator()
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["applyConverter", jindo.$Fn(this.applyConverter, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["addConverter", jindo.$Fn(this.addConverter, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["addConverter_DOM", jindo.$Fn(this.addConverter_DOM, this).bind()])
        },
        applyConverter: function(e, t, i) {
            var n, o = "@" + (new Date).getTime() + "@",
                s = new RegExp(o, "g"),
                r = {
                    sContents: o + t
                };
            if (i = i || document, this.oApp.exec("MSG_STRING_CONVERTER_STARTED", [e, r]), t = r.sContents, n = this.oConverters_DOM[e]) {
                var a = i.createElement("DIV");
                a.innerHTML = t;
                for (var l = 0; l < n.length; l++) n[l](a);
                t = a.innerHTML, a.parentNode && a.parentNode.removeChild(a), a = null, jindo.$Agent().navigator().ie && (o += "(\r\n)?", s = new RegExp(o, "g"))
            }
            if (n = this.oConverters[e])
                for (l = 0; l < n.length; l++) {
                    var h = n[l](t);
                    void 0 !== h && (t = h)
                }
            return r = {
                sContents: t
            }, this.oApp.exec("MSG_STRING_CONVERTER_ENDED", [e, r]), r.sContents = r.sContents.replace(s, ""), r.sContents
        },
        $ON_ADD_CONVERTER: function(e, t) {
            var i = this.oApp.aCallerStack;
            t.sPluginName = i[i.length - 2].name, this.addConverter(e, t)
        },
        $ON_ADD_CONVERTER_DOM: function(e, t) {
            var i = this.oApp.aCallerStack;
            t.sPluginName = i[i.length - 2].name, this.addConverter_DOM(e, t)
        },
        addConverter: function(e, t) {
            this.oConverters[e] || (this.oConverters[e] = []), this.oConverters[e][this.oConverters[e].length] = t
        },
        addConverter_DOM: function(e, t) {
            this.oConverters_DOM[e] || (this.oConverters_DOM[e] = []), this.oConverters_DOM[e][this.oConverters_DOM[e].length] = t
        }
    })
}, function(e, t) {
    nhn.husky.MessageManager = jindo.$Class({
        name: "MessageManager",
        _oMessageMapSet: {},
        _sDefaultLocale: "ko_KR",
        $init: function(e, t) {
            var i;
            switch (t) {
                case "ja_JP":
                    i = window.oMessageMap_ja_JP;
                    break;
                case "en_US":
                    i = window.oMessageMap_en_US;
                    break;
                case "zh_CN":
                    i = window.oMessageMap_zh_CN;
                    break;
                default:
                    i = e
            }
            i = i || e, this._sDefaultLocale = t || this._sDefaultLocale, this._setMessageMap(i, this._sDefaultLocale)
        },
        _setMessageMap: function(e, t) {
            t = t || this._sDefaultLocale, e && (this._oMessageMapSet[t] = e)
        },
        _getMessageMap: function(e) {
            return this._oMessageMapSet[e] || this._oMessageMapSet[this._sDefaultLocale] || {}
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["$MSG", jindo.$Fn(this.getMessage, this).bind()])
        },
        getMessage: function(e, t) {
            var i = this._getMessageMap(t);
            return i[e] ? unescape(i[e]) : e
        }
    })
}, function(e, t) {
    nhn.husky.LazyLoader = jindo.$Class({
        name: "LazyLoader",
        htMsgInfo: null,
        aLoadingInfo: null,
        $init: function(e) {
            this.htMsgInfo = {}, this.aLoadingInfo = [], this.aToDo = e
        },
        $ON_MSG_APP_READY: function() {
            for (var e = 0; e < this.aToDo.length; e++) {
                var t = this.aToDo[e];
                this._createBeforeHandlersAndSaveURLInfo(t.oMsgs, t.sURL, t.elTarget, t.htOptions)
            }
        },
        $LOCAL_BEFORE_ALL: function(e, t) {
            var i = e.replace("$BEFORE_", ""),
                n = this.htMsgInfo[i];
            return 1 == n.nLoadingStatus || (2 == n.nLoadingStatus ? this[e] = function() {
                return this._removeHandler(e), this.oApp.delayedExec(i, t, 0), !1
            } : (n.bLoadingStatus = 1, new jindo.$Ajax(n.sURL, {
                onload: jindo.$Fn(this._onload, this).bind(i, t)
            }).request())), !0
        },
        _onload: function(e, t, i) {
            4 == i._response.readyState ? (this.htMsgInfo[e].elTarget.innerHTML = i.text(), this.htMsgInfo[e].nLoadingStatus = 2, this._removeHandler("$BEFORE_" + e), this.oApp.exec("sMsg", t)) : this.oApp.exec(this.htMsgInfo[e].sFailureCallback, [])
        },
        _removeHandler: function(e) {
            delete this[e], this.oApp.createMessageMap(e)
        },
        _createBeforeHandlersAndSaveURLInfo: function(e, t, i, n) {
            var o = {
                sURL: t,
                elTarget: i,
                sSuccessCallback: (n = n || {}).sSuccessCallback,
                sFailureCallback: n.sFailureCallback,
                nLoadingStatus: 0
            };
            if (this.aLoadingInfo[this.aLoadingInfo.legnth] = o, !(e instanceof Array)) {
                var s = e;
                e = [];
                var r = {};
                for (var a in s)
                    if (a.match(/^\$(BEFORE|ON|AFTER)_(.+)$/)) {
                        var l = RegExp.$2;
                        if ("MSG_APP_READY" == l) continue;
                        r[l] || (e[e.length] = RegExp.$2, r[l] = !0)
                    }
            }
            for (var h = 0; h < e.length; h++) {
                var d = "$BEFORE_" + e[h];
                this[d] = function() {
                    return !1
                }, this.oApp.createMessageMap(d), this.htMsgInfo[e[h]] = o
            }
        }
    })
}, function(e, t) {
    nhn.husky.PopUpManager = {}, nhn.husky.PopUpManager._instance = null, nhn.husky.PopUpManager._pluginKeyCnt = 0, nhn.husky.PopUpManager.getInstance = function(e) {
        return null == this._instance && (this._instance = new function() {
            this._whtPluginWin = new jindo.$H, this._whtPlugin = new jindo.$H, this.addPlugin = function(e, t) {
                this._whtPlugin.add(e, t)
            }, this.getPlugin = function() {
                return this._whtPlugin
            }, this.getPluginWin = function() {
                return this._whtPluginWin
            }, this.openWindow = function(e) {
                var t = {
                    oApp: null,
                    sUrl: "",
                    sName: "popup",
                    sLeft: null,
                    sTop: null,
                    nWidth: 400,
                    nHeight: 400,
                    sProperties: null,
                    bScroll: !0
                };
                for (var i in e) t[i] = e[i];
                null == t.oApp && alert("팝업 요청시 옵션으로 oApp(허스키 reference) 값을 설정하셔야 합니다.");
                var n = t.sLeft || (screen.availWidth - t.nWidth) / 2,
                    o = t.sTop || (screen.availHeight - t.nHeight) / 2,
                    s = null != t.sProperties ? t.sProperties : "top=" + o + ",left=" + n + ",width=" + t.nWidth + ",height=" + t.nHeight + ",scrollbars=" + (t.bScroll ? "yes" : "no") + ",status=yes",
                    r = window.open(t.sUrl, t.sName, s);
                return r && setTimeout(function() {
                    try {
                        r.focus()
                    } catch (e) {}
                }, 100), this.removePluginWin(r), this._whtPluginWin.add(this.getCorrectKey(this._whtPlugin, t.oApp), r), r
            }, this.getCorrectKey = function(e, i) {
                var n = null;
                return e.forEach(function(e, t) {
                    e != i || (n = t)
                }), n
            }, this.removePluginWin = function(e) {
                var t = this._whtPluginWin.search(e);
                t && (this._whtPluginWin.remove(t), this.removePluginWin(e))
            }
        }), this._instance.addPlugin("plugin_" + this._pluginKeyCnt++, e), nhn.husky.PopUpManager._instance
    }, nhn.husky.PopUpManager.setCallback = function(e, t, i) {
        if (this._instance.getPluginWin().hasValue(e)) {
            var n = this._instance.getCorrectKey(this._instance.getPluginWin(), e);
            n && this._instance.getPlugin().$(n).exec(t, i)
        }
    }, nhn.husky.PopUpManager.getFunc = function(e, t) {
        if (this._instance.getPluginWin().hasValue(e)) {
            var i = this._instance.getCorrectKey(this._instance.getPluginWin(), e);
            if (i) return this._instance.getPlugin().$(i)[t]()
        }
    }
}, function(e, t) {
    var i, n, o, s, r, a, l;
    "undefined" == typeof window.nhn && (window.nhn = {}), nhn.husky || (nhn.husky = {}), i = navigator.userAgent, n = jindo.$Agent(), o = n.navigator(), s = n.os(), 2 === (r = i.match(/(SHW-|Chrome|Safari)/gi) || "").length && "SHW-" === r[0] && "Safari" === r[1] ? o.bGalaxyBrowser = !0 : -1 < i.indexOf("LG-V500") && -1 < i.indexOf("Version/4.0") && (o.bGPadBrowser = !0), "undefined" == typeof s.ios && (s.ios = -1 < i.indexOf("iPad") || -1 < i.indexOf("iPhone"), s.ios && null != (r = i.match(/(iPhone )?OS ([\d|_]+)/)) && r[2] != undefined && (s.version = String(r[2]).split("_").join("."))), nhn.husky.SE2M_UtilPlugin = jindo.$Class({
        name: "SE2M_UtilPlugin",
        $BEFORE_MSG_APP_READY: function() {
            this.oApp.exec("ADD_APP_PROPERTY", ["oAgent", jindo.$Agent()]), this.oApp.exec("ADD_APP_PROPERTY", ["oNavigator", jindo.$Agent().navigator()]), this.oApp.exec("ADD_APP_PROPERTY", ["oUtils", this])
        },
        $ON_REGISTER_HOTKEY: function(e, t, i, n) {
            this.oApp.exec("ADD_HOTKEY", [e, t, i, n || this.oApp.getWYSIWYGDocument()])
        },
        $ON_SE2_ATTACH_HOVER_EVENTS: function(e) {
            this.oApp.exec("ATTACH_HOVER_EVENTS", [e, {
                fnElmToSrc: this._elm2Src,
                fnElmToTarget: this._elm2Target
            }])
        },
        _elm2Src: function(e) {
            return "LI" == e.tagName && e.firstChild && "BUTTON" == e.firstChild.tagName ? e.firstChild : e
        },
        _elm2Target: function(e) {
            return "BUTTON" == e.tagName && "LI" == e.parentNode.tagName ? e.parentNode : e
        },
        getScrollXY: function() {
            var e, t, i = this.oApp.getWYSIWYGWindow();
            return t = "undefined" == typeof i.scrollX ? (e = i.document.documentElement.scrollLeft, i.document.documentElement.scrollTop) : (e = i.scrollX, i.scrollY), {
                x: e,
                y: t
            }
        }
    }), nhn.husky.SE2M_Utils = {
        sURLPattern: "(http|https|ftp|mailto):(?:\\/\\/)?((:?\\w|-)+(:?\\.(:?\\w|-)+)+)([^ <>]+)?",
        rxDateFormat: /^(?:\d{4}\.)?\d{1,2}\.\d{1,2}$/,
        _rxTable: /^(?:CAPTION|TBODY|THEAD|TFOOT|TR|TD|TH|COLGROUP|COL)$/i,
        _rxSpaceOnly: /^\s+$/,
        _rxFontStart: /<font(?:\s+[^>]*)?>/i,
        _rxFontStrip: /<\/?font(?:\s+[^>]*)?>/gi,
        _bUnderIE8: jindo.$Agent().navigator().ie && jindo.$Agent().navigator().version < 9,
        _htFontSize: {
            1: "7pt",
            2: "10pt",
            3: "12pt",
            4: "13.5pt",
            5: "18pt",
            6: "24pt",
            7: "36pt"
        },
        isInvalidNodeInTable: function(e) {
            if (e && !this._rxTable.test(e.nodeName)) {
                var t;
                if ((t = e.previousSibling) && this._rxTable.test(t.nodeName)) return !0;
                if ((t = e.nextSibling) && this._rxTable.test(t.nodeName)) return !0
            }
            return !1
        },
        removeInvalidNodeInTable: function(e) {
            this.isInvalidNodeInTable(e) && e.parentNode && e.parentNode.removeChild(e)
        },
        removeInvalidFont: function(e) {
            e && (this._removeInvalidFontInTable(e), this._removeEmptyFont(e))
        },
        _removeInvalidFontInTable: function(e) {
            for (var t, i = jindo.$$("table font", e), n = 0; t = i[n]; n++) this.removeInvalidNodeInTable(t)
        },
        _removeEmptyFont: function(e) {
            for (var t, i = jindo.$$("font", e), n = 0; t = i[n]; n++)(t.innerHTML || "").replace(this._rxSpaceOnly, "") || t.parentNode.removeChild(t)
        },
        convertFontToSpan: function(e) {
            if (e) {
                for (var t, i, n, o = e.ownerDocument || document, s = jindo.$$("font", e), r = 0; t = s[r]; r++)("SPAN" !== (i = t.parentNode).tagName || 1 < i.childNodes.length) && (i = o.createElement("SPAN"), t.parentNode.insertBefore(i, t)), (n = t.getAttribute("face")) && (i.style.fontFamily = n), (n = this._htFontSize[t.getAttribute("size")]) && (i.style.fontSize = n), (n = t.getAttribute("color")) && (i.style.color = n), this._switchFontInnerToSpan(t, i), t.parentNode && t.parentNode.removeChild(t);
                0 < r && this._bUnderIE8 && (e.innerHTML = e.innerHTML.replace(this._rxFontStrip, ""))
            }
        },
        _switchFontInnerToSpan: function(e, t) {
            var i = e.innerHTML;
            if (this._rxFontStart.test(i) || this._bUnderIE8)
                for (var n; n = e.firstChild;) t.appendChild(n);
            else t.innerHTML = i
        },
        stripTag: function(e) {
            for (var t; t = e.firstChild;) e.parentNode.insertBefore(t, e);
            e.parentNode.removeChild(e)
        },
        stripTags: function(e, t) {
            for (var i, n = jindo.$$(t, e), o = 0; i = n[o]; o++) this.stripTag(i)
        },
        reviseDateFormat: function(e) {
            return e && e.replace && (e = e.replace(this.rxDateFormat, "$&.")), e
        },
        getCustomCSS: function(e, t, i) {
            var n = [];
            if (void 0 === e || void 0 === t || !e || !t) return n;
            var o = e.match(t);
            return o && o[0] && o[1] && (i ? n = o[1].split(i) : n[0] = o[1]), n
        },
        toStringSamePropertiesOfArray: function(e, t, i) {
            if (!e) return "";
            if (e instanceof Array) {
                for (var n = [], o = 0; o < e.length; o++) n.push(e[o][t]);
                return n.join(",").replace(/,/g, i)
            }
            return "undefined" == typeof e[t] ? "" : "string" == typeof e[t] ? e[t] : void 0
        },
        makeArray: function(e) {
            if (null == e) return [];
            if (e instanceof Array) return e;
            var t = [];
            return t.push(e), t
        },
        ellipsis: function(e, t, i, n) {
            i = i || "...", void 0 === n && (n = 1);
            var o = jindo.$Element(e),
                s = jindo.$Element(t),
                r = o.html(),
                a = r.length,
                l = s.height(),
                h = 0;
            o.html("A");
            var d = s.height();
            if (l < d * (n + .5)) return o.html(r);
            for (l = d; l < d * (n + .5);) h += Math.max(Math.ceil((a - h) / 2), 1), o.html(r.substring(0, h) + i), l = s.height();
            for (; d * (n + .5) < l;) h--, o.html(r.substring(0, h) + i), l = s.height()
        },
        ellipsisByPixel: function(e, t, i, n) {
            t = t || "...";
            var o = jindo.$Element(e),
                s = o.width();
            if (!(s < i)) {
                var r = o.html(),
                    a = r.length,
                    l = 0;
                if (void 0 === n) {
                    for (s = o.html("A").width(); s < i;) l += Math.max(Math.ceil((a - l) / 2), 1), o.html(r.substring(0, l) + t), s = o.width();
                    n = function() {
                        return !0
                    }
                }
                for (l = o.html().length - t.length; i < s && n();) l--, o.html(r.substring(0, l) + t), s = o.width()
            }
        },
        ellipsisElementsToDesinatedWidth: function(e, i, n, o) {
            jindo.$A(e).forEach(function(e, t) {
                e || jindo.$A.Continue(), nhn.husky.SE2M_Utils.ellipsisByPixel(e, i, n[t], o)
            })
        },
        paddingZero: function(e, t) {
            for (var i = e.toString(); i.length < t;) i = "0" + i;
            return i
        },
        cutStringToByte: function(e, t, i) {
            if (null === e || 0 === e.length) return e;
            i || "" == i || (i = "...");
            for (var n = t, o = 0, s = (e = e.replace(/  +/g, " ")).length, r = 0; r < s; r++) {
                if ((o += this.getCharByte(e.charAt(r))) == n) return r == s - 1 ? e : e.substring(0, r) + i;
                if (n < o) return e.substring(0, r) + i
            }
            return e
        },
        getCharByte: function(e) {
            if (null === e || e.length < 1) return 0;
            var t = 0,
                i = escape(e);
            return 1 == i.length ? t++ : -1 != i.indexOf("%u") ? t += 2 : -1 != i.indexOf("%") && (t += i.length / 3), t
        },
        getFilteredHashTable: function(e, t) {
            if (!(t instanceof Array)) return arguments.callee.call(this, e, [t]);
            var i = jindo.$A(t);
            return jindo.$H(e).filter(function(e, t) {
                return !(!i.has(t) || !e)
            }).$value()
        },
        isBlankNode: function(e) {
            function n(e) {
                return !e || (!!o(e) || ("BR" == e.tagName || ("&nbsp;" == e.innerHTML || "" == e.innerHTML)))
            }

            function t(e) {
                if ("IMG" == e.tagName || "IFRAME" == e.tagName) return !1;
                if (n(e)) return !0;
                if ("P" == e.tagName) {
                    for (var t = e.childNodes.length - 1; 0 <= t; t--) {
                        var i = e.childNodes[t];
                        o(i) && i.parentNode.removeChild(i)
                    }
                    if (1 == e.childNodes.length) {
                        if ("IMG" == e.firstChild.tagName || "IFRAME" == e.firstChild.tagName) return !1;
                        if (n(e.firstChild)) return !0
                    }
                }
                return !1
            }
            var o = this.isBlankTextNode;
            if (t(e)) return !0;
            for (var i = 0, s = e.childNodes.length; i < s; i++) {
                if (!t(e.childNodes[i])) return !1
            }
            return !0
        },
        isBlankTextNode: function(e) {
            return 3 == e.nodeType && "" == e.nodeValue.replace(unescape("%uFEFF"), "")
        },
        isFirstChildOfNode: function(e, t, i) {
            return !!i && (i.tagName == t && i.firstChild.tagName == e)
        },
        findAncestorByTagName: function(e, t) {
            for (; t && t.tagName != e;) t = t.parentNode;
            return t
        },
        findAncestorByTagNameWithCount: function(e, t) {
            for (var i = 0; t && t.tagName != e;) t = t.parentNode, i += 1;
            return {
                elNode: t,
                nRecursiveCount: i
            }
        },
        findClosestAncestorAmongTagNames: function(e, t) {
            for (var i = new RegExp("^(" + e.join("|") + ")$", "i"); t && !i.test(t.tagName);) t = t.parentNode;
            return t
        },
        findClosestAncestorAmongTagNamesWithCount: function(e, t) {
            for (var i = 0, n = new RegExp("^(" + e.join("|") + ")$", "i"); t && !n.test(t.tagName);) t = t.parentNode, i += 1;
            return {
                elNode: t,
                nRecursiveCount: i
            }
        },
        isNumber: function(e) {
            return !isNaN(parseFloat(e)) && isFinite(e)
        },
        deleteProperty: function(e, t) {
            if ("object" == typeof e && "string" == typeof t && "undefined" != typeof e[t]) {
                e[t] = undefined;
                try {
                    delete e[t]
                } catch (i) {}
            }
        },
        loadCSS: function(e, t) {
            var i = document,
                n = i.getElementsByTagName("HEAD")[0],
                o = i.createElement("LINK");
            o.setAttribute("type", "text/css"), o.setAttribute("rel", "stylesheet"), o.setAttribute("href", e), t && ("onload" in o ? o.onload = function() {
                t(), this.onload = null
            } : o.onreadystatechange = function() {
                "complete" == o.readyState && (o.getAttribute("_complete") || (o.setAttribute("_complete", !0), t()))
            }), n.appendChild(o)
        },
        getUniqueId: function(e) {
            return (e || "") + jindo.$Date().time() + (1e5 * Math.random()).toFixed()
        },
        clone: function(e, t) {
            if (void 0 === e || !e || e.constructor != Array && e.constructor != Object) return e;
            var i = e.constructor == Array ? [] : {};
            for (var n in e) void 0 !== t && t[n] ? i[n] = arguments.callee(t[n]) : i[n] = arguments.callee(e[n]);
            return i
        },
        getHtmlTagAttr: function(e, t) {
            var i = new RegExp("\\s" + t + "=('([^']*)'|\"([^\"]*)\"|([^\"' >]*))", "i").exec(e);
            if (!i) return "";
            var n = i[1] || i[2] || i[3];
            return n = n && n.replace(/["]/g, "")
        },
        iframeAlignConverter: function(e, t) {
            var i = e.tagName.toUpperCase();
            if ("DIV" == i || "P" == i) {
                if (null === e.parentNode) return;
                var n = t,
                    o = jindo.$Element(e),
                    s = o.html(),
                    r = jindo.$Element(e).attr("align") || jindo.$Element(e).css("text-align"),
                    a = jindo.$Element(jindo.$("<div></div>", n));
                a.html(s).attr("align", r), o.replace(a)
            }
        },
        getJsonDatafromXML: function(e) {
            function g(e) {
                return void 0 === e ? "" : e.replace(/&[a-z]+;/g, function(e) {
                    return "string" == typeof i[e] ? i[e] : e
                })
            }

            function f(e) {
                for (var t in e)
                    if (Object.prototype.hasOwnProperty.call(e, t)) {
                        if (Object.prototype[t]) continue;
                        return !1
                    } return !0
            }
            var t = {},
                A = /^[0-9]+(?:\.[0-9]+)?$/,
                S = /^\s+$/g,
                i = {
                    "&amp;": "&",
                    "&nbsp;": " ",
                    "&quot;": '"',
                    "&lt;": "<",
                    "&gt;": ">"
                },
                T = {
                    tags: ["/"],
                    stack: [t]
                };
            return (e = e.replace(/<(\?|!-)[^>]*>/g, "")).replace(/\s*<(\/?[\w:-]+)((?:\s+[\w:-]+\s*=\s*(?:"(?:\\"|[^"])*"|'(?:\\'|[^'])*'))*)\s*((?:\/>)|(?:><\/\1>|\s*))|\s*<!\[CDATA\[([\w\W]*?)\]\]>\s*|\s*>?([^<]*)/gi, function(e, t, i, n, o, s) {
                var r, a = "",
                    l = T.stack.length - 1;
                if ("string" == typeof t && t)
                    if ("/" != t.substr(0, 1)) {
                        var h = "string" == typeof i && i,
                            d = "string" == typeof n && n,
                            c = !h && d ? "" : {};
                        if ("undefined" == typeof(r = T.stack[l])[t]) r[t] = c, r = T.stack[1 + l] = r[t];
                        else if (r[t] instanceof Array) {
                            var p = r[t].length;
                            r[t][p] = c, r = T.stack[1 + l] = r[t][p]
                        } else r[t] = [r[t], c], r = T.stack[1 + l] = r[t][1];
                        h && function(e, o) {
                            e.replace(/([\w:-]+)\s*=\s*(?:"((?:\\"|[^"])*)"|'((?:\\'|[^'])*)')/g, function(e, t, i, n) {
                                o[t] = g((i ? i.replace(/\\"/g, '"') : undefined) || (n ? n.replace(/\\'/g, "'") : undefined))
                            })
                        }(i, r), T.tags[1 + l] = t, d && (T.tags.length--, T.stack.length--)
                    } else T.tags.length--, T.stack.length--;
                else "string" == typeof o && o ? a = o : "string" == typeof s && s.replace(S, "") && (a = g(s));
                if (0 < a.length) {
                    var u = T.stack[l - 1],
                        _ = T.tags[l];
                    if (A.test(a) || "true" != a && "false" != a || (a = new Boolean(a)), void 0 === u) return;
                    if (u[_] instanceof Array) {
                        var E = u[_];
                        "object" != typeof E[E.length - 1] || f(E[E.length - 1]) ? E[E.length - 1] = a : (E[E.length - 1].$cdata = a, E[E.length - 1].toString = function() {
                            return a
                        })
                    } else "object" != typeof u[_] || f(u[_]) ? u[_] = a : (u[_].$cdata = a, u[_].toString = function() {
                        return a
                    })
                }
            }), jindo.$Json(t)
        },
        replaceSpecialChar: function(e) {
            return "string" == typeof e ? e.replace(/&/g, "&amp;").replace(/"/g, "&quot;").replace(/'/g, "&#39;").replace(/</g, "&lt;").replace(/>/g, "&gt;") : ""
        },
        restoreSpecialChar: function(e) {
            return "string" == typeof e ? e.replace(/&quot;/g, '"').replace(/&#39;/g, "'").replace(/&lt;/g, "<").replace(/&gt;/g, ">").replace(/&amp;/g, "&") : ""
        }
    }, nhn.husky.AutoResizer = jindo.$Class({
        welHiddenDiv: null,
        welCloneDiv: null,
        elContainer: null,
        $init: function(e, t) {
            var i = ["lineHeight", "textDecoration", "letterSpacing", "fontSize", "fontFamily", "fontStyle", "fontWeight", "textTransform", "textAlign", "direction", "wordSpacing", "fontSizeAdjust", "paddingTop", "paddingLeft", "paddingBottom", "paddingRight", "width"],
                n = i.length,
                o = {
                    position: "absolute",
                    top: -9999,
                    left: -9999,
                    opacity: 0,
                    overflow: "hidden",
                    wordWrap: "break-word"
                };
            for (this.nMinHeight = t.nMinHeight, this.wfnCallback = t.wfnCallback, this.elContainer = e.parentNode, this.welTextArea = jindo.$Element(e), this.welHiddenDiv = jindo.$Element("<div>"), this.wfnResize = jindo.$Fn(this._resize, this), this.sOverflow = this.welTextArea.css("overflow"), this.welTextArea.css("overflow", "hidden"); n--;) o[i[n]] = this.welTextArea.css(i[n]);
            this.welHiddenDiv.css(o), this.nLastHeight = this.welTextArea.height()
        },
        bind: function() {
            this.welCloneDiv = jindo.$Element(this.welHiddenDiv.$value().cloneNode(!1)), this.wfnResize.attach(this.welTextArea, "keyup"), this.welCloneDiv.appendTo(this.elContainer), this._resize()
        },
        unbind: function() {
            this.wfnResize.detach(this.welTextArea, "keyup"), this.welTextArea.css("overflow", this.sOverflow), this.welCloneDiv && this.welCloneDiv.leave()
        },
        _resize: function() {
            var e, t = this.welTextArea.$value().value,
                i = !1;
            t !== this.sContents && (this.sContents = t.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/ /g, "&nbsp;").replace(/\n/g, "<br>"), this.sContents += "<br>", this.welCloneDiv.html(this.sContents), (e = this.welCloneDiv.height()) < this.nMinHeight && (e = this.nMinHeight), this.welTextArea.css("height", e + "px"), this.elContainer.style.height = e + "px", this.nLastHeight < e && (i = !0), this.wfnCallback(i))
        }
    }), "undefined" == typeof window.StringBuffer && (window.StringBuffer = {}), StringBuffer = function(e) {
        this._aString = [], void 0 !== e && this.append(e)
    }, StringBuffer.prototype.append = function(e) {
        return this._aString.push(e), this
    }, StringBuffer.prototype.toString = function() {
        return this._aString.join("")
    }, StringBuffer.prototype.setLength = function(e) {
        this._aString.length = void 0 === e || e <= 0 ? 0 : e
    }, a = null, l = /,/gi, window.IsInstalledFont = function(e) {
        var t = "Comic Sans MS" == e ? "Courier New" : "Comic Sans MS",
            i = "position:absolute !important; font-size:200px !important; left:-9999px !important; top:-9999px !important;";
        (a = a || document.createElement("div")).innerHTML = "mmmmiiiii" + unescape("%uD55C%uAE00"), a.style.cssText = i + 'font-family:"' + t + '" !important';
        var n = document.body || document.documentElement;
        n.firstChild ? n.insertBefore(a, n.firstChild) : document.body.appendChild(a);
        var o = a.offsetWidth + "-" + a.offsetHeight;
        a.style.cssText = i + 'font-family:"' + e.replace(l, '","') + '", "' + t + '" !important';
        var s = o != a.offsetWidth + "-" + a.offsetHeight;
        return document.body.removeChild(a), s
    }
}]);