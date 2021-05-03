/*! Copyright (C) NAVER corp. Licensed under LGPL v2. @see https://github.com/naver/smarteditor2/blob/master/LICENSE.md */ ! function(n) {
    var i = {};

    function o(t) {
        if (i[t]) return i[t].exports;
        var e = i[t] = {
            i: t,
            l: !1,
            exports: {}
        };
        return n[t].call(e.exports, e, e.exports, o), e.l = !0, e.exports
    }
    o.m = n, o.c = i, o.d = function(t, e, n) {
        o.o(t, e) || Object.defineProperty(t, e, {
            enumerable: !0,
            get: n
        })
    }, o.r = function(t) {
        "undefined" != typeof Symbol && Symbol.toStringTag && Object.defineProperty(t, Symbol.toStringTag, {
            value: "Module"
        }), Object.defineProperty(t, "__esModule", {
            value: !0
        })
    }, o.t = function(e, t) {
        if (1 & t && (e = o(e)), 8 & t) return e;
        if (4 & t && "object" == typeof e && e && e.__esModule) return e;
        var n = Object.create(null);
        if (o.r(n), Object.defineProperty(n, "default", {
                enumerable: !0,
                value: e
            }), 2 & t && "string" != typeof e)
            for (var i in e) o.d(n, i, function(t) {
                return e[t]
            }.bind(null, i));
        return n
    }, o.n = function(t) {
        var e = t && t.__esModule ? function() {
            return t["default"]
        } : function() {
            return t
        };
        return o.d(e, "a", e), e
    }, o.o = function(t, e) {
        return Object.prototype.hasOwnProperty.call(t, e)
    }, o.p = "", o(o.s = 0)
}([function(t, e, n) {
    "use strict";
    n(1), n(2), n(3), n(4)
}, function(t, e) {
    var d, r, l, a;
    "undefined" == typeof window.nhn && (window.nhn = {}), nhn.husky || (nhn.husky = {}), d = /^\$(LOCAL|BEFORE|ON|AFTER)_/, r = /^\$(BEFORE|ON|AFTER)_MSG_APP_READY$/, l = [], a = {}, nhn.husky.HuskyCore = jindo.$Class({
        name: "HuskyCore",
        aCallerStack: null,
        bMobile: jindo.$Agent().navigator().mobile || jindo.$Agent().navigator().msafari,
        $init: function(t) {
            this.htOptions = t || {}, l.push(this), this.htOptions.oDebugger && (nhn.husky.HuskyCore.getCore = function() {
                return l
            }, this.htOptions.oDebugger.setApp(this)), this.messageQueue = [], this.oMessageMap = {}, this.oDisabledMessage = {}, this.oLazyMessage = {}, this.aPlugins = [], this.appStatus = nhn.husky.APP_STATUS.NOT_READY, this.aCallerStack = [], this._fnWaitForPluginReady = jindo.$Fn(this._waitForPluginReady, this).bind(), this.registerPlugin(this)
        },
        setDebugger: function(t) {
            (this.htOptions.oDebugger = t).setApp(this)
        },
        exec: function(t, e, n) {
            if (this.appStatus == nhn.husky.APP_STATUS.NOT_READY) return this.messageQueue[this.messageQueue.length] = {
                msg: t,
                args: e,
                event: n
            }, !0;
            this.exec = this._exec, this.exec(t, e, n)
        },
        delayedExec: function(t, e, n, i) {
            var o = jindo.$Fn(this.exec, this).bind(t, e, i);
            setTimeout(o, n)
        },
        _exec: function(t, e, n) {
            return (this._exec = this.htOptions.oDebugger ? this._execWithDebugger : this._execWithoutDebugger).call(this, t, e, n)
        },
        _execWithDebugger: function(t, e, n) {
            this.htOptions.oDebugger.log_MessageStart(t, e);
            var i = this._doExec(t, e, n);
            return this.htOptions.oDebugger.log_MessageEnd(t, e), i
        },
        _execWithoutDebugger: function(t, e, n) {
            return this._doExec(t, e, n)
        },
        _doExec: function(t, e, n) {
            var i = !1;
            if (this.oLazyMessage[t]) {
                var o = this.oLazyMessage[t];
                return this._loadLazyFiles(t, e, n, o.aFilenames, 0), !1
            }
            if (!this.oDisabledMessage[t]) {
                var s = [];
                if (e && e.length)
                    for (var r = e.length, a = 0; a < r; a++) s[a] = e[a];
                n && (s[s.length] = n), i = (i = (i = this._execMsgStep("BEFORE", t, s)) && this._execMsgStep("ON", t, s)) && this._execMsgStep("AFTER", t, s)
            }
            return i
        },
        registerPlugin: function(t) {
            if (!t) throw "An error occured in registerPlugin(): invalid plug-in";
            if (t.nIdx = this.aPlugins.length, ((t.oApp = this).aPlugins[t.nIdx] = t).status != nhn.husky.PLUGIN_STATUS.NOT_READY && (t.status = nhn.husky.PLUGIN_STATUS.READY), this.appStatus != nhn.husky.APP_STATUS.NOT_READY)
                for (var e in t) d.test(e) && this.addToMessageMap(e, t);
            return this.exec("MSG_PLUGIN_REGISTERED", [t]), t.nIdx
        },
        disableMessage: function(t, e) {
            this.oDisabledMessage[t] = e
        },
        registerBrowserEvent: function(t, e, n, i, o) {
            i = i || [];
            var s = o ? jindo.$Fn(this.delayedExec, this).bind(n, i, o) : jindo.$Fn(this.exec, this).bind(n, i);
            return jindo.$Fn(s, this).attach(t, e)
        },
        run: function(t) {
            this.htRunOptions = t || {}, this._changeAppStatus(nhn.husky.APP_STATUS.WAITING_FOR_PLUGINS_READY);
            for (var e = this.messageQueue.length, n = 0; n < e; n++) {
                var i = this.messageQueue[n];
                this.exec(i.msg, i.args, i.event)
            }
            this._fnWaitForPluginReady()
        },
        acceptLocalBeforeFirstAgain: function(t, e) {
            t._husky_bRun = !e
        },
        createMessageMap: function(t) {
            this.oMessageMap[t] = [];
            for (var e = this.aPlugins.length, n = 0; n < e; n++) this._doAddToMessageMap(t, this.aPlugins[n])
        },
        addToMessageMap: function(t, e) {
            this.oMessageMap[t] && this._doAddToMessageMap(t, e)
        },
        _changeAppStatus: function(t) {
            this.appStatus = t, this.appStatus == nhn.husky.APP_STATUS.READY && this.exec("MSG_APP_READY")
        },
        _execMsgStep: function(t, e, n) {
            return (this._execMsgStep = this.htOptions.oDebugger ? this._execMsgStepWithDebugger : this._execMsgStepWithoutDebugger).call(this, t, e, n)
        },
        _execMsgStepWithDebugger: function(t, e, n) {
            this.htOptions.oDebugger.log_MessageStepStart(t, e, n);
            var i = this._execMsgHandler("$" + t + "_" + e, n);
            return this.htOptions.oDebugger.log_MessageStepEnd(t, e, n), i
        },
        _execMsgStepWithoutDebugger: function(t, e, n) {
            return this._execMsgHandler("$" + t + "_" + e, n)
        },
        _execMsgHandler: function(t, e) {
            var n;
            this.oMessageMap[t] || this.createMessageMap(t);
            var i = this.oMessageMap[t],
                o = i.length;
            if (0 === o) return !0;
            var s = !0;
            if (r.test(t)) {
                for (n = 0; n < o; n++)
                    if (!1 === this._execHandler(i[n], t, e)) {
                        s = !1;
                        break
                    }
            } else
                for (n = 0; n < o; n++)
                    if ((i[n]._husky_bRun || (i[n]._husky_bRun = !0, "function" != typeof i[n].$LOCAL_BEFORE_FIRST || !1 !== this._execHandler(i[n], "$LOCAL_BEFORE_FIRST", [t, e]))) && ("function" != typeof i[n].$LOCAL_BEFORE_ALL || !1 !== this._execHandler(i[n], "$LOCAL_BEFORE_ALL", [t, e])) && !1 === this._execHandler(i[n], t, e)) {
                        s = !1;
                        break
                    } return s
        },
        _execHandler: function(t, e, n) {
            return (this._execHandler = this.htOptions.oDebugger ? this._execHandlerWithDebugger : this._execHandlerWithoutDebugger).call(this, t, e, n)
        },
        _execHandlerWithDebugger: function(t, e, n) {
            var i;
            this.htOptions.oDebugger.log_CallHandlerStart(t, e, n);
            try {
                this.aCallerStack.push(t), i = t[e].apply(t, n), this.aCallerStack.pop()
            } catch (o) {
                this.htOptions.oDebugger.handleException(o), i = !1
            }
            return this.htOptions.oDebugger.log_CallHandlerEnd(t, e, n), i
        },
        _execHandlerWithoutDebugger: function(t, e, n) {
            this.aCallerStack.push(t);
            var i = t[e].apply(t, n);
            return this.aCallerStack.pop(), i
        },
        _doAddToMessageMap: function(t, e) {
            if ("function" == typeof e[t]) {
                for (var n = 0, i = this.oMessageMap[t].length; n < i; n++)
                    if (this.oMessageMap[t][n] == e) return;
                this.oMessageMap[t][n] = e
            }
        },
        _waitForPluginReady: function() {
            for (var t = !0, e = 0; e < this.aPlugins.length; e++)
                if (this.aPlugins[e].status == nhn.husky.PLUGIN_STATUS.NOT_READY) {
                    t = !1;
                    break
                } t ? this._changeAppStatus(nhn.husky.APP_STATUS.READY) : setTimeout(this._fnWaitForPluginReady, 100)
        },
        _loadLazyFiles: function(t, e, n, i, o) {
            if (i.length <= o) return this.oLazyMessage[t] = null, void this.oApp.exec(t, e, n);
            var s = i[o];
            a[s] ? this._loadLazyFiles(t, e, n, i, o + 1) : jindo.LazyLoading.load(nhn.husky.SE2M_Configuration.LazyLoad.sJsBaseURI + "/" + s, jindo.$Fn(function(t, e, n, i, o) {
                var s = i[o];
                a[s] = 1, this._loadLazyFiles(t, e, n, i, o + 1)
            }, this).bind(t, e, n, i, o), "utf-8")
        },
        registerLazyMessage: function(t, e) {
            t = t || [], e = e || [];
            for (var n, i, o = 0; n = t[o]; o++)(i = this.oLazyMessage[n]) ? i.aFilenames = i.aFilenames.concat(e) : this.oLazyMessage[n] = {
                sMsg: n,
                aFilenames: e
            }
        }
    }), nhn.husky.HuskyCore._htLoadedFile = {}, nhn.husky.HuskyCore.addLoadedFile = function(t) {
        a[t] = 1
    }, nhn.husky.HuskyCore.mixin = function(t, e, n) {
        var i, o, s, r, a, h = [];
        for (i = 0; r = l[i]; i++)
            for (o = 0; a = r.aPlugins[o]; o++)
                if (a instanceof t) h.push(a), "function" != typeof a.$LOCAL_BEFORE_FIRST && a.oApp.acceptLocalBeforeFirstAgain(a, !0);
                else if (a._$superClass === t)
            for (s in "function" != typeof a.$LOCAL_BEFORE_FIRST && a.oApp.acceptLocalBeforeFirstAgain(a, !0), e) !n && Object.prototype.hasOwnProperty.call(a, s) || (a[s] = e[s], d.test(s) && a.oApp.addToMessageMap(s, a));
        for (s in e)
            if ((n || !Object.prototype.hasOwnProperty.call(t.prototype, s)) && (t.prototype[s] = e[s], d.test(s)))
                for (o = 0; a = h[o]; o++) a.oApp.addToMessageMap(s, a)
    }, nhn.husky.APP_STATUS = {
        NOT_READY: 0,
        WAITING_FOR_PLUGINS_READY: 1,
        READY: 2
    }, nhn.husky.PLUGIN_STATUS = {
        NOT_READY: 0,
        READY: 1
    }
}, function(t, e) {
    "undefined" == typeof window.nhn && (window.nhn = {}), nhn.CurrentSelection_IE = function() {
        this.getCommonAncestorContainer = function() {
            try {
                return this._oSelection = this._document.selection, "Control" == this._oSelection.type ? this._oSelection.createRange().item(0) : this._oSelection.createRangeCollection().item(0).parentElement()
            } catch (t) {
                return this._document.body
            }
        }, this.isCollapsed = function() {
            return this._oSelection = this._document.selection, "None" == this._oSelection.type
        }
    }, nhn.CurrentSelection_FF = function() {
        this.getCommonAncestorContainer = function() {
            return this._getSelection().commonAncestorContainer
        }, this.isCollapsed = function() {
            var t = this._window.getSelection();
            return t.rangeCount < 1 || t.getRangeAt(0).collapsed
        }, this._getSelection = function() {
            try {
                return this._window.getSelection().getRangeAt(0)
            } catch (t) {
                return this._document.createRange()
            }
        }
    }, nhn.CurrentSelection = new(jindo.$Class({
        $init: function() {
            jindo.$Agent().navigator().ie && document.selection ? nhn.CurrentSelection_IE.apply(this) : nhn.CurrentSelection_FF.apply(this)
        },
        setWindow: function(t) {
            this._window = t, this._document = t.document
        }
    })), nhn.W3CDOMRange = jindo.$Class({
        $init: function(t) {
            this.reset(t)
        },
        reset: function(t) {
            this._window = t, this._document = this._window.document, this.collapsed = !0, this.commonAncestorContainer = this._document.body, this.endContainer = this._document.body, this.endOffset = 0, this.startContainer = this._document.body, this.startOffset = 0, this.oBrowserSelection = new nhn.BrowserSelection(this._window), this.selectionLoaded = this.oBrowserSelection.selectionLoaded
        },
        cloneContents: function() {
            var t = this._document.createDocumentFragment(),
                e = this._document.createDocumentFragment(),
                n = this._getNodesInRange();
            if (n.length < 1) return t;
            var i = this._constructClonedTree(n, e),
                o = e.firstChild;
            if (o)
                for (var s, r = o.firstChild; r;) s = r.nextSibling, t.appendChild(r), r = s;
            return (i = this._splitTextEndNodes({
                oStartContainer: i.oStartContainer,
                iStartOffset: this.startOffset,
                oEndContainer: i.oEndContainer,
                iEndOffset: this.endOffset
            })).oStartContainer && i.oStartContainer.previousSibling && nhn.DOMFix.parentNode(i.oStartContainer).removeChild(i.oStartContainer.previousSibling), i.oEndContainer && i.oEndContainer.nextSibling && nhn.DOMFix.parentNode(i.oEndContainer).removeChild(i.oEndContainer.nextSibling), t
        },
        _constructClonedTree: function(t, e) {
            var s = null,
                r = null,
                a = this.startContainer,
                h = this.endContainer;
            return this._recurConstructClonedTree = function(t, e, n) {
                if (e < 0) return e;
                var i = e - 1,
                    o = t[e].cloneNode(!1);
                for (t[e] == a && (s = o), t[e] == h && (r = o); 0 <= i && nhn.DOMFix.parentNode(t[i]) == t[e];) i = this._recurConstructClonedTree(t, i, o);
                return n.insertBefore(o, n.firstChild), i
            }, t[t.length] = nhn.DOMFix.parentNode(t[t.length - 1]), this._recurConstructClonedTree(t, t.length - 1, e), {
                oStartContainer: s,
                oEndContainer: r
            }
        },
        cloneRange: function() {
            return this._copyRange(new nhn.W3CDOMRange(this._window))
        },
        _copyRange: function(t) {
            return t.collapsed = this.collapsed, t.commonAncestorContainer = this.commonAncestorContainer, t.endContainer = this.endContainer, t.endOffset = this.endOffset, t.startContainer = this.startContainer, t.startOffset = this.startOffset, t._document = this._document, t
        },
        collapse: function(t) {
            t ? (this.endContainer = this.startContainer, this.endOffset = this.startOffset) : (this.startContainer = this.endContainer, this.startOffset = this.endOffset), this._updateRangeInfo()
        },
        compareBoundaryPoints: function(t, e) {
            switch (t) {
                case nhn.W3CDOMRange.START_TO_START:
                    return this._compareEndPoint(this.startContainer, this.startOffset, e.startContainer, e.startOffset);
                case nhn.W3CDOMRange.START_TO_END:
                    return this._compareEndPoint(this.endContainer, this.endOffset, e.startContainer, e.startOffset);
                case nhn.W3CDOMRange.END_TO_END:
                    return this._compareEndPoint(this.endContainer, this.endOffset, e.endContainer, e.endOffset);
                case nhn.W3CDOMRange.END_TO_START:
                    return this._compareEndPoint(this.startContainer, this.startOffset, e.endContainer, e.endOffset)
            }
        },
        _findBody: function(t) {
            if (!t) return null;
            for (; t;) {
                if ("BODY" == t.tagName) return t;
                t = nhn.DOMFix.parentNode(t)
            }
            return null
        },
        _compareEndPoint: function(t, e, n, i) {
            return this.oBrowserSelection.compareEndPoints(t, e, n, i)
        },
        _getCommonAncestorContainer: function(t, e) {
            t = t || this.startContainer;
            for (var n = e = e || this.endContainer; t;) {
                for (; n;) {
                    if (t == n) return t;
                    n = nhn.DOMFix.parentNode(n)
                }
                n = e, t = nhn.DOMFix.parentNode(t)
            }
            return this._document.body
        },
        deleteContents: function() {
            if (!this.collapsed) {
                this._splitTextEndNodesOfTheRange();
                var t = this._getNodesInRange();
                if (!(t.length < 1)) {
                    for (var e = t[0].previousSibling; e && this._isBlankTextNode(e);) e = e.previousSibling;
                    var n, i = -1;
                    e || (n = nhn.DOMFix.parentNode(t[0]), i = 0);
                    for (var o = 0; o < t.length; o++) {
                        var s = t[o];
                        !s.firstChild || this._isAllChildBlankText(s) ? (n == s && (i = this._getPosIdx(n), n = nhn.DOMFix.parentNode(s)), s.parentNode.removeChild(s)) : n == s && 0 === i && (i = this._getPosIdx(n), n = nhn.DOMFix.parentNode(s))
                    }
                    e ? "BODY" == e.tagName ? this.setStartBefore(e, !0) : this.setStartAfter(e, !0) : this.setStart(n, i, !0, !0), this.collapse(!0)
                }
            }
        },
        extractContents: function() {
            var t = this.cloneContents();
            return this.deleteContents(), t
        },
        getInsertBeforeNodes: function() {
            var t, e = null;
            return (e = "3" == this.startContainer.nodeType ? (t = nhn.DOMFix.parentNode(this.startContainer), this.startContainer.nodeValue.length <= this.startOffset ? this.startContainer.nextSibling : this.startContainer.splitText(this.startOffset)) : (t = this.startContainer, nhn.DOMFix.childNodes(this.startContainer)[this.startOffset])) && nhn.DOMFix.parentNode(e) || (e = null), {
                elParent: t,
                elBefore: e
            }
        },
        insertNode: function(t) {
            var e = this.getInsertBeforeNodes();
            e.elParent.insertBefore(t, e.elBefore), this.setStartBefore(t)
        },
        selectNode: function(t) {
            this.reset(this._window), this.setStartBefore(t), this.setEndAfter(t)
        },
        selectNodeContents: function(t) {
            this.reset(this._window), this.setStart(t, 0, !0), this.setEnd(t, nhn.DOMFix.childNodes(t).length)
        },
        _endsNodeValidation: function(t, e) {
            if (!t || this._findBody(t) != this._document.body) throw new Error("INVALID_NODE_TYPE_ERR oNode is not part of current document");
            return 3 == t.nodeType ? e > t.nodeValue.length && (e = t.nodeValue.length) : e > nhn.DOMFix.childNodes(t).length && (e = nhn.DOMFix.childNodes(t).length), e
        },
        setEnd: function(t, e, n, i) {
            n || (e = this._endsNodeValidation(t, e)), this.endContainer = t, this.endOffset = e, i || (this.startContainer && -1 == this._compareEndPoint(this.startContainer, this.startOffset, this.endContainer, this.endOffset) ? this._updateRangeInfo() : this.collapse(!1))
        },
        setEndAfter: function(t, e) {
            if (!t) throw new Error("INVALID_NODE_TYPE_ERR in setEndAfter");
            "BODY" != t.tagName ? this.setEnd(nhn.DOMFix.parentNode(t), this._getPosIdx(t) + 1, !0, e) : this.setEnd(t, nhn.DOMFix.childNodes(t).length, !0, e)
        },
        setEndBefore: function(t, e) {
            if (!t) throw new Error("INVALID_NODE_TYPE_ERR in setEndBefore");
            "BODY" != t.tagName ? this.setEnd(nhn.DOMFix.parentNode(t), this._getPosIdx(t), !0, e) : this.setEnd(t, 0, !0, e)
        },
        setStart: function(t, e, n, i) {
            n || (e = this._endsNodeValidation(t, e)), this.startContainer = t, this.startOffset = e, i || (this.endContainer && -1 == this._compareEndPoint(this.startContainer, this.startOffset, this.endContainer, this.endOffset) ? this._updateRangeInfo() : this.collapse(!0))
        },
        setStartAfter: function(t, e) {
            if (!t) throw new Error("INVALID_NODE_TYPE_ERR in setStartAfter");
            "BODY" != t.tagName ? this.setStart(nhn.DOMFix.parentNode(t), this._getPosIdx(t) + 1, !0, e) : this.setStart(t, nhn.DOMFix.childNodes(t).length, !0, e)
        },
        setStartBefore: function(t, e) {
            if (!t) throw new Error("INVALID_NODE_TYPE_ERR in setStartBefore");
            "BODY" != t.tagName ? this.setStart(nhn.DOMFix.parentNode(t), this._getPosIdx(t), !0, e) : this.setStart(t, 0, !0, e)
        },
        surroundContents: function(t) {
            t.appendChild(this.extractContents()), this.insertNode(t), this.selectNode(t)
        },
        toString: function() {
            var t = this._document.createElement("DIV");
            return t.appendChild(this.cloneContents()), t.textContent || t.innerText || ""
        },
        fixCommonAncestorContainer: function() {
            jindo.$Agent().navigator().ie && (this.commonAncestorContainer = this._getCommonAncestorContainer())
        },
        _isBlankTextNode: function(t) {
            return 3 == t.nodeType && "" == t.nodeValue
        },
        _isAllChildBlankText: function(t) {
            for (var e = 0, n = t.childNodes.length; e < n; e++)
                if (!this._isBlankTextNode(t.childNodes[e])) return !1;
            return !0
        },
        _getPosIdx: function(t) {
            for (var e = 0, n = t.previousSibling; n; n = n.previousSibling) e++;
            return e
        },
        _updateRangeInfo: function() {
            this.startContainer ? (this.collapsed = this.oBrowserSelection.isCollapsed(this) || this.startContainer === this.endContainer && this.startOffset === this.endOffset, this.commonAncestorContainer = this.oBrowserSelection.getCommonAncestorContainer(this)) : this.reset(this._window)
        },
        _isCollapsed: function(t, e, n, i) {
            var o = !1;
            if (t == n && e == i) o = !0;
            else {
                var s = this._getActualStartNode(t, e),
                    r = this._getActualEndNode(n, i);
                s = this._getNextNode(this._getPrevNode(s)), r = this._getPrevNode(this._getNextNode(r)), s && r && "BODY" != r.tagName && (this._getNextNode(r) == s || r == s && this._isBlankTextNode(r)) && (o = !0)
            }
            return o
        },
        _splitTextEndNodesOfTheRange: function() {
            var t = this._splitTextEndNodes({
                oStartContainer: this.startContainer,
                iStartOffset: this.startOffset,
                oEndContainer: this.endContainer,
                iEndOffset: this.endOffset
            });
            this.startContainer = t.oStartContainer, this.startOffset = t.iStartOffset, this.endContainer = t.oEndContainer, this.endOffset = t.iEndOffset
        },
        _splitTextEndNodes: function(t) {
            return t = this._splitStartTextNode(t), t = this._splitEndTextNode(t)
        },
        _splitStartTextNode: function(t) {
            var e = t.oStartContainer,
                n = t.iStartOffset,
                i = t.oEndContainer,
                o = t.iEndOffset;
            if (!e) return t;
            if (3 != e.nodeType) return t;
            if (0 === n) return t;
            if (e.nodeValue.length <= n) return t;
            var s = e.splitText(n);
            return e == i && (o -= n, i = s), {
                oStartContainer: e = s,
                iStartOffset: n = 0,
                oEndContainer: i,
                iEndOffset: o
            }
        },
        _splitEndTextNode: function(t) {
            var e = t.oStartContainer,
                n = t.iStartOffset,
                i = t.oEndContainer,
                o = t.iEndOffset;
            return i ? 3 != i.nodeType ? t : o >= i.nodeValue.length ? t : 0 === o ? t : (i.splitText(o), {
                oStartContainer: e,
                iStartOffset: n,
                oEndContainer: i,
                iEndOffset: o
            }) : t
        },
        _getNodesInRange: function() {
            if (this.collapsed) return [];
            var t = this._getActualStartNode(this.startContainer, this.startOffset),
                e = this._getActualEndNode(this.endContainer, this.endOffset);
            return this._getNodesBetween(t, e)
        },
        _getActualStartNode: function(t, e) {
            var n = t;
            return 3 == t.nodeType ? e >= t.nodeValue.length ? "BODY" == (n = this._getNextNode(t)).tagName && (n = null) : n = t : e < nhn.DOMFix.childNodes(t).length ? n = nhn.DOMFix.childNodes(t)[e] : "BODY" == (n = this._getNextNode(t)).tagName && (n = null), n
        },
        _getActualEndNode: function(t, e) {
            var n = t;
            return 0 === e ? "BODY" == (n = this._getPrevNode(t)).tagName && (n = null) : n = 3 == t.nodeType ? t : nhn.DOMFix.childNodes(t)[e - 1], n
        },
        _getNextNode: function(t) {
            return t && "BODY" != t.tagName ? t.nextSibling ? t.nextSibling : this._getNextNode(nhn.DOMFix.parentNode(t)) : this._document.body
        },
        _getPrevNode: function(t) {
            return t && "BODY" != t.tagName ? t.previousSibling ? t.previousSibling : this._getPrevNode(nhn.DOMFix.parentNode(t)) : this._document.body
        },
        _getNodesBetween: function(t, e) {
            var n = [];
            if (this._nNodesBetweenLen = 0, !t || !e) return n;
            try {
                this._recurGetNextNodesUntil(t, e, n)
            } catch (i) {
                return []
            }
            return n
        },
        _recurGetNextNodesUntil: function(t, e, n) {
            if (!t) return !1;
            if (!this._recurGetChildNodesUntil(t, e, n)) return !1;
            for (var i = t.nextSibling; !i;) {
                if (!(t = nhn.DOMFix.parentNode(t))) return !1;
                if ((n[this._nNodesBetweenLen++] = t) == e) return !1;
                i = t.nextSibling
            }
            return this._recurGetNextNodesUntil(i, e, n)
        },
        _recurGetChildNodesUntil: function(t, e, n) {
            if (!t) return !1;
            var i = !1,
                o = t;
            if (o.firstChild)
                for (o = o.firstChild; o;) {
                    if (!this._recurGetChildNodesUntil(o, e, n)) {
                        i = !0;
                        break
                    }
                    o = o.nextSibling
                }
            return n[this._nNodesBetweenLen++] = t, !i && t != e
        }
    }), nhn.W3CDOMRange.START_TO_START = 0, nhn.W3CDOMRange.START_TO_END = 1, nhn.W3CDOMRange.END_TO_END = 2, nhn.W3CDOMRange.END_TO_START = 3, nhn.HuskyRange = jindo.$Class({
        _rxCursorHolder: /^(?:\uFEFF|\u00A0|\u200B|<br>)$/i,
        _rxTextAlign: /text-align:[^"';]*;?/i,
        setWindow: function(t) {
            this.reset(t || window)
        },
        $init: function(t) {
            this.HUSKY_BOOMARK_START_ID_PREFIX = "husky_bookmark_start_", this.HUSKY_BOOMARK_END_ID_PREFIX = "husky_bookmark_end_", this.sBlockElement = "P|DIV|LI|H[1-6]|PRE", this.sBlockContainer = "BODY|TABLE|TH|TR|TD|UL|OL|BLOCKQUOTE|FORM", this.rxBlockElement = new RegExp("^(" + this.sBlockElement + ")$"), this.rxBlockContainer = new RegExp("^(" + this.sBlockContainer + ")$"), this.rxLineBreaker = new RegExp("^(" + this.sBlockElement + "|" + this.sBlockContainer + ")$"), this.rxHasBlock = new RegExp("(?:<(?:" + this.sBlockElement + "|" + this.sBlockContainer + ").*?>|style=[\"']?[^>]*?(?:display\\s?:\\s?block)[^>]*?[\"']?)", "i"), this.setWindow(t)
        },
        select: function() {
            try {
                this.oBrowserSelection.selectRange(this)
            } catch (t) {}
        },
        setFromSelection: function(t) {
            this.setRange(this.oBrowserSelection.getRangeAt(t), !0)
        },
        setRange: function(t, e) {
            this.reset(this._window), this.setStart(t.startContainer, t.startOffset, e, !0), this.setEnd(t.endContainer, t.endOffset, e)
        },
        setEndNodes: function(t, e) {
            this.reset(this._window), this.setEndAfter(e, !0), this.setStartBefore(t)
        },
        splitTextAtBothEnds: function() {
            this._splitTextEndNodesOfTheRange()
        },
        getStartNode: function() {
            return this.collapsed ? 3 == this.startContainer.nodeType ? 0 === this.startOffset ? null : this.startContainer.nodeValue.length <= this.startOffset ? null : this.startContainer : null : 3 == this.startContainer.nodeType ? this.startOffset >= this.startContainer.nodeValue.length ? this._getNextNode(this.startContainer) : this.startContainer : this.startOffset >= nhn.DOMFix.childNodes(this.startContainer).length ? this._getNextNode(this.startContainer) : nhn.DOMFix.childNodes(this.startContainer)[this.startOffset]
        },
        getEndNode: function() {
            return this.collapsed ? this.getStartNode() : 3 == this.endContainer.nodeType ? 0 === this.endOffset ? this._getPrevNode(this.endContainer) : this.endContainer : 0 === this.endOffset ? this._getPrevNode(this.endContainer) : nhn.DOMFix.childNodes(this.endContainer)[this.endOffset - 1]
        },
        getNodeAroundRange: function(t, e) {
            return this.collapsed ? this.startContainer && 3 == this.startContainer.nodeType ? this.startContainer : (i = this.startOffset >= nhn.DOMFix.childNodes(this.startContainer).length ? this._getNextNode(this.startContainer) : nhn.DOMFix.childNodes(this.startContainer)[this.startOffset], n = 0 === this.endOffset ? this._getPrevNode(this.endContainer) : nhn.DOMFix.childNodes(this.endContainer)[this.endOffset - 1], t ? (o = n) || e || (o = i) : (o = i) || e || (o = n), o) : this.getStartNode();
            var n, i, o
        },
        _getXPath: function(t) {
            for (var e = ""; t && 1 == t.nodeType;) e = "/" + t.tagName + "[" + this._getPosIdx4XPath(t) + "]" + e, t = nhn.DOMFix.parentNode(t);
            return e
        },
        _getPosIdx4XPath: function(t) {
            for (var e = 0, n = t.previousSibling; n; n = n.previousSibling) n.tagName == t.tagName && e++;
            return e
        },
        _evaluateXPath: function(t, e) {
            for (var n = (t = t.substring(1, t.length - 1)).split(/\//), i = e.body, o = 2; o < n.length && i; o++) {
                n[o].match(/([^[]+)\[(\d+)/i);
                for (var s = RegExp.$1, r = RegExp.$2, a = nhn.DOMFix.childNodes(i), h = [], d = a.length, l = 0, u = 0; u < d; u++) a[u].tagName == s && (h[l++] = a[u]);
                i = h.length < r ? null : h[r]
            }
            return i
        },
        _evaluateXPathBookmark: function(t) {
            var e = t.sXPath,
                n = t.nTextNodeIdx,
                i = t.nOffset,
                o = this._evaluateXPath(e, this._document);
            if (-1 < n && o) {
                for (var s = nhn.DOMFix.childNodes(o), r = null, a = n, h = i;
                    (r = s[a]) && 3 == r.nodeType && r.nodeValue.length < h;) h -= r.nodeValue.length, a++;
                o = nhn.DOMFix.childNodes(o)[a], i = h
            }
            return o || (o = this._document.body, i = 0), {
                elContainer: o,
                nOffset: i
            }
        },
        getXPathBookmark: function() {
            var t = -1,
                e = {
                    elContainer: this.startContainer,
                    nOffset: this.startOffset
                },
                n = this.startContainer;
            3 == n.nodeType && (e = this._getFixedStartTextNode(), t = this._getPosIdx(e.elContainer), n = nhn.DOMFix.parentNode(n));
            var i, o = this._getXPath(n),
                s = {
                    sXPath: o,
                    nTextNodeIdx: t,
                    nOffset: e.nOffset
                };
            if (this.collapsed) i = {
                sXPath: o,
                nTextNodeIdx: t,
                nOffset: e.nOffset
            };
            else {
                var r = -1,
                    a = {
                        elContainer: this.endContainer,
                        nOffset: this.endOffset
                    },
                    h = this.endContainer;
                3 == h.nodeType && (a = this._getFixedEndTextNode(), r = this._getPosIdx(a.elContainer), h = nhn.DOMFix.parentNode(h)), i = {
                    sXPath: this._getXPath(h),
                    nTextNodeIdx: r,
                    nOffset: a.nOffset
                }
            }
            return [s, i]
        },
        moveToXPathBookmark: function(t) {
            if (!t) return !1;
            var e = this._evaluateXPathBookmark(t[0]),
                n = this._evaluateXPathBookmark(t[1]);
            return e.elContainer && n.elContainer ? (this.startContainer = e.elContainer, this.startOffset = e.nOffset, this.endContainer = n.elContainer, this.endOffset = n.nOffset, !0) : void 0
        },
        _getFixedTextContainer: function(t, e) {
            for (; t && 3 == t.nodeType && t.previousSibling && 3 == t.previousSibling.nodeType;) e += t.previousSibling.nodeValue.length, t = t.previousSibling;
            return {
                elContainer: t,
                nOffset: e
            }
        },
        _getFixedStartTextNode: function() {
            return this._getFixedTextContainer(this.startContainer, this.startOffset)
        },
        _getFixedEndTextNode: function() {
            return this._getFixedTextContainer(this.endContainer, this.endOffset)
        },
        placeStringBookmark: function() {
            return this.collapsed || jindo.$Agent().navigator().ie || jindo.$Agent().navigator().firefox ? this.placeStringBookmark_NonWebkit() : this.placeStringBookmark_Webkit()
        },
        placeStringBookmark_NonWebkit: function() {
            var t = (new Date).getTime(),
                e = this.cloneRange();
            e.collapseToEnd();
            var n = this._document.createElement("SPAN");
            n.id = this.HUSKY_BOOMARK_END_ID_PREFIX + t, e.insertNode(n), (e = this.cloneRange()).collapseToStart();
            var i = this._document.createElement("SPAN");
            if (i.id = this.HUSKY_BOOMARK_START_ID_PREFIX + t, e.insertNode(i), jindo.$Agent().navigator().ie) {
                try {
                    i.innerHTML = unescape("%uFEFF")
                } catch (o) {}
                try {
                    n.innerHTML = unescape("%uFEFF")
                } catch (o) {}
            }
            return this.moveToBookmark(t), t
        },
        placeStringBookmark_Webkit: function() {
            var t, e, n = (new Date).getTime(),
                i = this.cloneRange();
            i.collapseToEnd(), t = this._document.createTextNode(""), i.insertNode(t), e = t.parentNode, t.previousSibling && "TD" == t.previousSibling.tagName && (e = t.previousSibling, t = null);
            var o = this._document.createElement("SPAN");
            o.id = this.HUSKY_BOOMARK_END_ID_PREFIX + n, e.insertBefore(o, t), (i = this.cloneRange()).collapseToStart(), t = this._document.createTextNode(""), i.insertNode(t), e = t.parentNode, t.nextSibling && "TD" == t.nextSibling.tagName && (t = (e = t.nextSibling).firstChild);
            var s = this._document.createElement("SPAN");
            return s.id = this.HUSKY_BOOMARK_START_ID_PREFIX + n, e.insertBefore(s, t), this.moveToBookmark(n), n
        },
        cloneRange: function() {
            return this._copyRange(new nhn.HuskyRange(this._window))
        },
        moveToBookmark: function(t) {
            return "object" != typeof t ? this.moveToStringBookmark(t) : this.moveToXPathBookmark(t)
        },
        getStringBookmark: function(t, e) {
            return e ? this._document.getElementById(this.HUSKY_BOOMARK_END_ID_PREFIX + t) : this._document.getElementById(this.HUSKY_BOOMARK_START_ID_PREFIX + t)
        },
        moveToStringBookmark: function(t, e) {
            var n = this.getStringBookmark(t),
                i = this.getStringBookmark(t, !0);
            return !(!n || !i) && (this.reset(this._window), e ? (this.setEndAfter(i), this.setStartBefore(n)) : (this.setEndBefore(i), this.setStartAfter(n)), !0)
        },
        removeStringBookmark: function(t) {
            this._removeAll(this.HUSKY_BOOMARK_START_ID_PREFIX + t), this._removeAll(this.HUSKY_BOOMARK_END_ID_PREFIX + t)
        },
        _removeAll: function(t) {
            for (var e; e = this._document.getElementById(t);) e.parentNode.removeChild(e)
        },
        collapseToStart: function() {
            this.collapse(!0)
        },
        collapseToEnd: function() {
            this.collapse(!1)
        },
        createAndInsertNode: function(t) {
            var e = this._document.createElement(t);
            return this.insertNode(e), e
        },
        getNodes: function(t, e) {
            t && this._splitTextEndNodesOfTheRange();
            var n = this._getNodesInRange(),
                i = [];
            if (!e) return n;
            for (var o = 0; o < n.length; o++) e(n[o]) && (i[i.length] = n[o]);
            return i
        },
        getTextNodes: function(t) {
            return this.getNodes(t, function(t) {
                return 3 == t.nodeType && "\n" != t.nodeValue && "" != t.nodeValue
            })
        },
        surroundContentsWithNewNode: function(t) {
            var e = this._document.createElement(t);
            return this.surroundContents(e), e
        },
        isRangeinRange: function(t, e) {
            var n = this.compareBoundaryPoints(this.W3CDOMRange.START_TO_START, t),
                i = this.compareBoundaryPoints(this.W3CDOMRange.START_TO_END, t),
                o = this.compareBoundaryPoints(this.W3CDOMRange.ND_TO_START, t),
                s = this.compareBoundaryPoints(this.W3CDOMRange.END_TO_END, t);
            return n <= 0 && 0 <= s || !!e && (1 != i && -1 != o)
        },
        isNodeInRange: function(t, e, n) {
            var i = new nhn.HuskyRange(this._window);
            return n && t.firstChild ? (i.setStartBefore(t.firstChild), i.setEndAfter(t.lastChild)) : i.selectNode(t), this.isRangeInRange(i, e)
        },
        pasteText: function(t) {
            this.pasteHTML(t.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/ /g, "&nbsp;").replace(/"/g, "&quot;"))
        },
        pasteHTML: function(t, e) {
            var n = this._document.createElement("DIV");
            if (n.innerHTML = t, n.firstChild) {
                var i = this.cloneRange(),
                    o = i.placeStringBookmark(),
                    s = i.getLineInfo(),
                    r = s.oStart,
                    a = s.oEnd;
                if (r.oLineBreaker && "P" === r.oLineBreaker.nodeName && (e || i.rxHasBlock.test(t))) {
                    var h = r.oLineBreaker.parentNode,
                        d = r.oLineBreaker.nextSibling;
                    if (i.deleteContents(), r.oLineBreaker === a.oLineBreaker) {
                        var l = i.getStringBookmark(o);
                        i.setEndNodes(l, a.oLineBreaker);
                        var u = i.extractContents(),
                            f = u.firstChild;
                        d ? h.insertBefore(u, d) : h.appendChild(u), d = f
                    }
                    n.style.cssText = r.oLineBreaker.style.cssText.replace(this._rxTextAlign, ""), n.align = r.oLineBreaker.align, d ? h.insertBefore(n, d) : h.appendChild(n), i.removeStringBookmark(o), this._removeEmptyP(this._getPrevElement(n));
                    var c = this._getNextElement(n);
                    if (c) {
                        var g = this._getNextElement(c);
                        g && this._removeEmptyP(c) && (c = g)
                    } else(c = this._document.createElement("P")).style.cssText = r.oLineBreaker.style.cssText, c.align = r.oLineBreaker.align, h.appendChild(c);
                    "" === c.innerHTML && (c.innerHTML = jindo.$Agent().navigator().ie && 8 < jindo.$Agent().navigator().version ? "​" : "\ufeff"), this.selectNodeContents(c), this.collapseToStart(), jindo.$Agent().navigator().ie && jindo.$Agent().navigator().version < 9 && (o = this.placeStringBookmark(), this.removeStringBookmark(o))
                } else {
                    var _ = n.firstChild,
                        p = n.lastChild;
                    for (this.collapseToStart(); n.lastChild;) this.insertNode(n.lastChild);
                    this.setEndNodes(_, p), i.moveToBookmark(o), i.deleteContents(), i.removeStringBookmark(o)
                }
            } else this.deleteContents()
        },
        _removeEmptyP: function(t) {
            if (t && "P" === t.nodeName) {
                var e = t.innerHTML;
                if ("" === e || this._rxCursorHolder.test(e)) return t.parentNode.removeChild(t), !0
            }
        },
        _getSiblingElement: function(t, e) {
            if (!t) return null;
            var n = t[e ? "previousSibling" : "nextSibling"];
            return n && 1 === n.nodeType ? n : arguments.callee(n, e)
        },
        _getPrevElement: function(t) {
            return this._getSiblingElement(t, !0)
        },
        _getNextElement: function(t) {
            return this._getSiblingElement(t, !1)
        },
        toString: function() {
            return this.toString = nhn.W3CDOMRange.prototype.toString, this.toString()
        },
        toHTMLString: function() {
            var t = this._document.createElement("DIV");
            return t.appendChild(this.cloneContents()), t.innerHTML
        },
        findAncestorByTagName: function(t) {
            for (var e = this.commonAncestorContainer; e && e.tagName != t;) e = nhn.DOMFix.parentNode(e);
            return e
        },
        selectNodeContents: function(t) {
            if (t) {
                var e = t.firstChild ? t.firstChild : t,
                    n = t.lastChild ? t.lastChild : t;
                this.reset(this._window), 3 == e.nodeType ? this.setStart(e, 0, !0) : this.setStartBefore(e), 3 == n.nodeType ? this.setEnd(n, n.nodeValue.length, !0) : this.setEndAfter(n)
            }
        },
        _hasTextDecoration: function(t, e) {
            return !(!t || !t.style) && (-1 < t.style.textDecoration.indexOf(e) || ("underline" === e && "U" === t.tagName || "line-through" === e && ("S" === t.tagName || "STRIKE" === t.tagName)))
        },
        _setTextDecoration: function(t, e) {
            jindo.$Agent().navigator().firefox ? t.style.textDecoration = t.style.textDecoration ? t.style.textDecoration + " " + e : e : "underline" === e ? t.innerHTML = "<U>" + t.innerHTML + "</U>" : "line-through" === e && (t.innerHTML = "<STRIKE>" + t.innerHTML + "</STRIKE>")
        },
        _checkTextDecoration: function(t) {
            if ("SPAN" === t.tagName) {
                for (var e = !1, n = !1, i = null, o = t.firstChild; o;) {
                    if (1 === o.nodeType && (e = e || "U" === o.tagName, n = n || "S" === o.tagName || "STRIKE" === o.tagName), e && n) return;
                    o = o.nextSibling
                }
                for (i = nhn.DOMFix.parentNode(t); i && "BODY" !== i.tagName;)
                    if (1 === i.nodeType) {
                        if (!e && this._hasTextDecoration(i, "underline") && (e = !0, this._setTextDecoration(t, "underline")), !n && this._hasTextDecoration(i, "line-through") && (n = !0, this._setTextDecoration(t, "line-through")), e && n) return;
                        i = nhn.DOMFix.parentNode(i)
                    } else i = nhn.DOMFix.parentNode(i)
            }
        },
        styleRange: function(t, e, n, i, o) {
            var s = this.aStyleParents = this._getStyleParentNodes(n, i);
            if (!(s.length < 1)) {
                for (var r, a, h, d = 0; d < s.length; d++) {
                    for (h in t) "string" == typeof(a = t[r = h]) && (o && t.color && this._checkTextDecoration(s[d]), s[d].style[r] = a);
                    if (e)
                        for (h in e) "string" == typeof(a = e[r = h]) && ("class" == r ? jindo.$Element(s[d]).addClass(a) : s[d].setAttribute(r, a))
                }
                this.reset(this._window), this.setStartBefore(s[0]), this.setEndAfter(s[s.length - 1])
            }
        },
        expandBothEnds: function() {
            this.expandStart(), this.expandEnd()
        },
        expandStart: function() {
            if (3 != this.startContainer.nodeType || 0 === this.startOffset) {
                var t = this._getActualStartNode(this.startContainer, this.startOffset);
                "BODY" == (t = this._getPrevNode(t)).tagName ? this.setStartBefore(t) : this.setStartAfter(t)
            }
        },
        expandEnd: function() {
            if (!(3 == this.endContainer.nodeType && this.endOffset < this.endContainer.nodeValue.length)) {
                var t = this._getActualEndNode(this.endContainer, this.endOffset);
                "BODY" == (t = this._getNextNode(t)).tagName ? this.setEndAfter(t) : this.setEndBefore(t)
            }
        },
        _getStyleParentNodes: function(t, e) {
            this._splitTextEndNodesOfTheRange();
            var n, i, o = this.getStartNode(),
                s = this.getEndNode(),
                r = this._getNodesInRange(),
                a = [],
                h = 0,
                d = r.length,
                l = jindo.$A(r).filter(function(t) {
                    return !t.firstChild || e && "LI" == t.tagName
                }),
                u = this.commonAncestorContainer;
            if (e)
                for (; u;) {
                    if ("LI" == u.tagName) {
                        this._isFullyContained(u, l) && (a[h++] = u);
                        break
                    }
                    u = u.parentNode
                }
            for (var f = 0; f < d; f++)
                if (n = r[f])
                    if (e && "LI" == n.tagName && this._isFullyContained(n, l)) a[h++] = n;
                    else if (3 == n.nodeType && "" != n.nodeValue && !n.nodeValue.match(/^(\r|\n)+$/)) {
                var c = nhn.DOMFix.parentNode(n);
                if ("SPAN" == c.tagName) {
                    if (this._isFullyContained(c, l, n)) {
                        a[h++] = c;
                        continue
                    }
                } else {
                    var g = this._findParentSingleSpan(c);
                    if (g) {
                        a[h++] = g;
                        continue
                    }
                }
                i = this._document.createElement("SPAN"), c.insertBefore(i, n), i.appendChild(n), a[h++] = i, t && i.setAttribute(t, "true")
            }
            return this.setStartBefore(o), this.setEndAfter(s), a
        },
        _findParentSingleSpan: function(t) {
            if (!t) return null;
            for (var e, n, i = 0, o = 0, s = t.childNodes; n = s[i]; i++)
                if (e = n.nodeValue, !this._rxCursorHolder.test(e) && 1 < ++o) return null;
            return "SPAN" === t.nodeName ? t : this._findParentSingleSpan(t.parentNode)
        },
        _isFullyContained: function(t, e, n) {
            var i, o, s = this._getVeryFirstRealChild(t);
            return -1 != (i = n && s == n ? 1 : e.indexOf(s)) && (s = this._getVeryLastRealChild(t), o = n && s == n ? 1 : e.indexOf(s)), -1 != i && -1 != o
        },
        _getVeryFirstChild: function(t) {
            return t.firstChild ? this._getVeryFirstChild(t.firstChild) : t
        },
        _getVeryLastChild: function(t) {
            return t.lastChild ? this._getVeryLastChild(t.lastChild) : t
        },
        _getFirstRealChild: function(t) {
            for (var e = t.firstChild; e && 3 == e.nodeType && "" == e.nodeValue;) e = e.nextSibling;
            return e
        },
        _getLastRealChild: function(t) {
            for (var e = t.lastChild; e && 3 == e.nodeType && "" == e.nodeValue;) e = e.previousSibling;
            return e
        },
        _getVeryFirstRealChild: function(t) {
            var e = this._getFirstRealChild(t);
            return e ? this._getVeryFirstRealChild(e) : t
        },
        _getVeryLastRealChild: function(t) {
            var e = this._getLastRealChild(t);
            return e ? this._getVeryLastChild(e) : t
        },
        _getLineStartInfo: function(t) {
            var n = null,
                i = t,
                o = t,
                s = !1,
                r = this.rxLineBreaker;
            return r.test(t.tagName) ? n = t : function a(t) {
                if (t && !n) {
                    if (r.test(t.tagName)) return o = t, n = i, void(s = !0);
                    (function e(t) {
                        if (t && !n) {
                            if (r.test(t.tagName)) return o = t, n = i, void(s = !1);
                            i = t, n || e(t.previousSibling)
                        }
                    })((i = t).previousSibling), n || a(nhn.DOMFix.parentNode(t))
                }
            }(t), {
                oNode: n,
                oLineBreaker: o,
                bParentBreak: s
            }
        },
        _getLineEndInfo: function(t) {
            var n = null,
                i = t,
                o = t,
                s = !1,
                r = this.rxLineBreaker;
            return r.test(t.tagName) ? n = t : function a(t) {
                if (t && !n) {
                    if (r.test(t.tagName)) return o = t, n = i, void(s = !0);
                    (function e(t) {
                        if (t && !n) {
                            if (r.test(t.tagName)) return o = t, n = i, void(s = !1);
                            i = t, n || e(t.nextSibling)
                        }
                    })((i = t).nextSibling), n || a(nhn.DOMFix.parentNode(t))
                }
            }(t), {
                oNode: n,
                oLineBreaker: o,
                bParentBreak: s
            }
        },
        getLineInfo: function(t) {
            t = t || !1;
            var e = this.getStartNode(),
                n = this.getEndNode();
            e = e || this.getNodeAroundRange(!t, !0), n = n || this.getNodeAroundRange(!t, !0);
            var i = this._getLineStartInfo(e),
                o = i.oNode,
                s = this._getLineEndInfo(n),
                r = s.oNode;
            if (e != o || n != r) {
                var a = this._compareEndPoint(nhn.DOMFix.parentNode(o), this._getPosIdx(o), this.endContainer, this.endOffset),
                    h = this._compareEndPoint(nhn.DOMFix.parentNode(r), this._getPosIdx(r) + 1, this.startContainer, this.startOffset);
                a <= 0 && 0 <= h || (e = this.getNodeAroundRange(!1, !0), n = this.getNodeAroundRange(!1, !0), i = this._getLineStartInfo(e), s = this._getLineEndInfo(n))
            }
            return {
                oStart: i,
                oEnd: s
            }
        },
        _findSingleChild: function(t) {
            if (!t) return null;
            for (var e, n, i = null, o = 0, s = 0, r = t.childNodes; n = r[o]; o++)
                if (e = n.nodeValue, !this._rxCursorHolder.test(e) && (i = n, 1 < ++s)) return null;
            return i
        },
        _hasCursorHolderOnly: function(t) {
            return !(!t || 1 !== t.nodeType) && (!!this._rxCursorHolder.test(t.innerHTML) || this._hasCursorHolderOnly(this._findSingleChild(t)))
        }
    }).extend(nhn.W3CDOMRange), nhn.BrowserSelection = function(t) {
        this.init = function(t) {
            this._window = t || window, this._document = this._window.document
        }, this.init(t), this._document.createRange ? nhn.BrowserSelectionImpl_FF.apply(this) : nhn.BrowserSelectionImpl_IE.apply(this), this.selectRange = function(t) {
            this.selectNone(), this.addRange(t)
        }, this.selectionLoaded = !0, this._oSelection || (this.selectionLoaded = !1)
    }, nhn.BrowserSelectionImpl_FF = function() {
        this._oSelection = this._window.getSelection(), this.getRangeAt = function(t) {
            t = t || 0;
            try {
                var e = this._oSelection.getRangeAt(t)
            } catch (n) {
                return new nhn.W3CDOMRange(this._window)
            }
            return this._FFRange2W3CRange(e)
        }, this.addRange = function(t) {
            var e = this._W3CRange2FFRange(t);
            this._oSelection.addRange(e)
        }, this.selectNone = function() {
            this._oSelection.removeAllRanges()
        }, this.getCommonAncestorContainer = function(t) {
            return this._W3CRange2FFRange(t).commonAncestorContainer
        }, this.isCollapsed = function(t) {
            return this._W3CRange2FFRange(t).collapsed
        }, this.compareEndPoints = function(t, e, n, i) {
            var o = this._document.createRange(),
                s = this._document.createRange();
            o.setStart(t, e), s.setStart(n, i), o.collapse(!0), s.collapse(!0);
            try {
                return o.compareBoundaryPoints(1, s)
            } catch (r) {
                return 1
            }
        }, this._FFRange2W3CRange = function(t) {
            var e = new nhn.W3CDOMRange(this._window);
            return e.setStart(t.startContainer, t.startOffset, !0), e.setEnd(t.endContainer, t.endOffset, !0), e
        }, this._W3CRange2FFRange = function(t) {
            var e = this._document.createRange();
            return e.setStart(t.startContainer, t.startOffset), e.setEnd(t.endContainer, t.endOffset), e
        }
    }, nhn.BrowserSelectionImpl_IE = function() {
        this._oSelection = this._document.selection, this.oLastRange = {
            oBrowserRange: null,
            elStartContainer: null,
            nStartOffset: -1,
            elEndContainer: null,
            nEndOffset: -1
        }, this._updateLastRange = function(t, e) {
            this.oLastRange.oBrowserRange = t, this.oLastRange.elStartContainer = e.startContainer, this.oLastRange.nStartOffset = e.startOffset, this.oLastRange.elEndContainer = e.endContainer, this.oLastRange.nEndOffset = e.endOffset
        }, this.getRangeAt = function(t) {
            var e, n, i;
            return t = t || 0, "Control" == this._oSelection.type ? (e = new nhn.W3CDOMRange(this._window), (i = this._oSelection.createRange().item(t)) && i.ownerDocument == this._document && e.selectNode(i), e) : e = (i = (n = this._oSelection.createRange()).parentElement()) && i.ownerDocument == this._document ? this._IERange2W3CRange(n) : new nhn.W3CDOMRange(this._window)
        }, this.addRange = function(t) {
            this._W3CRange2IERange(t).select()
        }, this.selectNone = function() {
            this._oSelection.empty()
        }, this.getCommonAncestorContainer = function(t) {
            return this._W3CRange2IERange(t).parentElement()
        }, this.isCollapsed = function(t) {
            var e = this._W3CRange2IERange(t),
                n = e.duplicate();
            return n.collapse(), e.isEqual(n)
        }, this.compareEndPoints = function(t, e, n, i) {
            var o, s;
            return t === this.oLastRange.elStartContainer && e === this.oLastRange.nStartOffset ? (o = this.oLastRange.oBrowserRange.duplicate()).collapse(!0) : t === this.oLastRange.elEndContainer && e === this.oLastRange.nEndOffset ? (o = this.oLastRange.oBrowserRange.duplicate()).collapse(!1) : o = this._getIERangeAt(t, e), n === this.oLastRange.elStartContainer && i === this.oLastRange.nStartOffset ? (s = this.oLastRange.oBrowserRange.duplicate()).collapse(!0) : n === this.oLastRange.elEndContainer && i === this.oLastRange.nEndOffset ? (s = this.oLastRange.oBrowserRange.duplicate()).collapse(!1) : s = this._getIERangeAt(n, i), o.compareEndPoints("StartToStart", s)
        }, this._W3CRange2IERange = function(t) {
            if (this.oLastRange.elStartContainer === t.startContainer && this.oLastRange.nStartOffset === t.startOffset && this.oLastRange.elEndContainer === t.endContainer && this.oLastRange.nEndOffset === t.endOffset) return this.oLastRange.oBrowserRange;
            var e = this._getIERangeAt(t.startContainer, t.startOffset),
                n = this._getIERangeAt(t.endContainer, t.endOffset);
            return e.setEndPoint("EndToEnd", n), this._updateLastRange(e, t), e
        }, this._getIERangeAt = function(t, e) {
            var n = this._document.body.createTextRange(),
                i = this._getSelectableNodeAndOffsetForIE(t, e),
                o = i.oSelectableNodeForIE,
                s = i.iOffsetForIE;
            return n.moveToElementText(o), n.collapse(i.bCollapseToStart), n.moveStart("character", s), n
        }, this._getSelectableNodeAndOffsetForIE = function(t, e) {
            var n = null,
                i = null,
                o = 0;
            o = 3 == t.nodeType ? (n = nhn.DOMFix.parentNode(t), (i = nhn.DOMFix.childNodes(n)).length) : (n = t, e < (i = nhn.DOMFix.childNodes(n)).length ? e : i.length);
            for (var s = null, r = 0, a = !0, h = 0; h < o; h++)
                if (3 == (s = i[h]).nodeType) {
                    if (s == t) break;
                    r += s.nodeValue.length
                } else n = s, r = 0, a = !1;
            return 3 == t.nodeType && (r += e), {
                oSelectableNodeForIE: n,
                iOffsetForIE: r,
                bCollapseToStart: a
            }
        }, this._IERange2W3CRange = function(t) {
            var e = new nhn.W3CDOMRange(this._window),
                n = null,
                i = null;
            (n = t.duplicate()).collapse(!0), i = this._getW3CContainerAndOffset(n, !0), e.setStart(i.oContainer, i.iOffset, !0, !0);
            var o = t.duplicate();
            return o.collapse(!0), o.isEqual(t) ? e.collapse(!0) : ((n = t.duplicate()).collapse(!1), i = this._getW3CContainerAndOffset(n), e.setEnd(i.oContainer, i.iOffset, !0)), this._updateLastRange(t, e), e
        }, this._getW3CContainerAndOffset = function(t, e) {
            for (var n, i = t, o = i.parentElement(), s = -1, r = this._document.body.createTextRange(), a = nhn.DOMFix.childNodes(o), h = null, d = 0; d < a.length; d++)
                if (3 != a[d].nodeType) {
                    if (r.moveToElementText(a[d]), 0 <= r.compareEndPoints("StartToStart", t)) break;
                    h = a[d]
                } if (0 !== (n = d) && 3 == a[n - 1].nodeType) {
                var l = this._document.body.createTextRange(),
                    u = null;
                u = h ? (l.moveToElementText(h), l.collapse(!1), h.nextSibling) : (l.moveToElementText(o), l.collapse(!0), o.firstChild);
                var f = i.duplicate();
                f.setEndPoint("StartToStart", l);
                for (var c = f.text.replace(/[\r\n]/g, "").length; c > u.nodeValue.length && u.nextSibling;) c -= u.nodeValue.length, u = u.nextSibling;
                u.nodeValue, e && u.nextSibling && 3 == u.nextSibling.nodeType && c == u.nodeValue.length && (c -= u.nodeValue.length, u = u.nextSibling), o = u, s = c
            } else o = i.parentElement(), s = n;
            return {
                oContainer: o,
                iOffset: s
            }
        }
    }, nhn.DOMFix = new(jindo.$Class({
        $init: function() {
            jindo.$Agent().navigator().ie || jindo.$Agent().navigator().opera ? (this.childNodes = this._childNodes_Fix, this.parentNode = this._parentNode_Fix) : (this.childNodes = this._childNodes_Native, this.parentNode = this._parentNode_Native)
        },
        _parentNode_Native: function(t) {
            return t.parentNode
        },
        _parentNode_Fix: function(t) {
            if (!t) return t;
            for (; t.previousSibling;) t = t.previousSibling;
            return t.parentNode
        },
        _childNodes_Native: function(t) {
            return t.childNodes
        },
        _childNodes_Fix: function(t) {
            var e = null,
                n = 0;
            if (t)
                for (e = [], t = t.firstChild; t;) t = (e[n++] = t).nextSibling;
            return e
        }
    }))
}, function(module, exports) {
    nhn.husky.CorePlugin = jindo.$Class({
        name: "CorePlugin",
        htLazyLoadRequest_plugins: {},
        htLazyLoadRequest_allFiles: {},
        htHTMLLoaded: {},
        $AFTER_MSG_APP_READY: function() {
            this.oApp.exec("EXEC_ON_READY_FUNCTION", [])
        },
        $ON_ADD_APP_PROPERTY: function(t, e) {
            this.oApp[t] = e
        },
        $ON_REGISTER_BROWSER_EVENT: function(t, e, n, i, o) {
            this.oApp.registerBrowserEvent(t, e, n, i, o)
        },
        $ON_DISABLE_MESSAGE: function(t) {
            this.oApp.disableMessage(t, !0)
        },
        $ON_ENABLE_MESSAGE: function(t) {
            this.oApp.disableMessage(t, !1)
        },
        $ON_LOAD_FULL_PLUGIN: function(t, e, n, i, o) {
            var s = t[0];
            this.htLazyLoadRequest_plugins[s] || (this.htLazyLoadRequest_plugins[s] = {
                nStatus: 1,
                sContents: ""
            }), 2 === this.htLazyLoadRequest_plugins[s].nStatus ? this.oApp.exec("MSG_FULL_PLUGIN_LOADED", [s, e, n, i, o, !1]) : this._loadFullPlugin(t, e, n, i, o, 0)
        },
        _loadFullPlugin: function(t, e, n, i, o, s) {
            jindo.LazyLoading.load(nhn.husky.SE2M_Configuration.LazyLoad.sJsBaseURI + "/" + t[s], jindo.$Fn(function(t, e, n, i, o, s) {
                var r = t[0];
                if (s == t.length - 1) return this.htLazyLoadRequest_plugins[r].nStatus = 2, void this.oApp.exec("MSG_FULL_PLUGIN_LOADED", [t, e, n, i, o]);
                this._loadFullPlugin(t, e, n, i, o, s + 1)
            }, this).bind(t, e, n, i, o, s), "utf-8")
        },
        $ON_MSG_FULL_PLUGIN_LOADED: function(aFilenames, sClassName, sMsgName, oThisRef, oArguments) {
            for (var oPluginRef = oThisRef.$this || oThisRef, i = 0, nLen = oThisRef._huskyFLT.length; i < nLen; i++) {
                var sLoaderHandlerName = "$BEFORE_" + oThisRef._huskyFLT[i],
                    oRemoveFrom = oThisRef.$this && oThisRef[sLoaderHandlerName] ? oThisRef : oPluginRef;
                oRemoveFrom[sLoaderHandlerName] = null, this.oApp.createMessageMap(sLoaderHandlerName)
            }
            var oPlugin = eval(sClassName + ".prototype"),
                bAcceptLocalBeforeFirstAgain = !1;
            for (var x in "function" != typeof oPluginRef.$LOCAL_BEFORE_FIRST && this.oApp.acceptLocalBeforeFirstAgain(oPluginRef, !0), oPlugin) oThisRef.$this && (!oThisRef[x] || "function" == typeof oPlugin[x] && "constructor" != x) && (oThisRef[x] = jindo.$Fn(oPlugin[x], oPluginRef).bind()), oPlugin[x] && (!oPluginRef[x] || "function" == typeof oPlugin[x] && "constructor" != x) && (oPluginRef[x] = oPlugin[x], x.match(/^\$(LOCAL|BEFORE|ON|AFTER)_/) && this.oApp.addToMessageMap(x, oPluginRef));
            bAcceptLocalBeforeFirstAgain && this.oApp.acceptLocalBeforeFirstAgain(oPluginRef, !0), oThisRef.$this || this.oApp.exec(sMsgName, oArguments)
        },
        $ON_LOAD_HTML: function(t) {
            if (!this.htHTMLLoaded[t]) {
                var e = jindo.$("_llh_" + t);
                if (e) {
                    this.htHTMLLoaded[t] = !0;
                    var n = document.createElement("DIV");
                    for (n.innerHTML = e.value; n.firstChild;) e.parentNode.insertBefore(n.firstChild, e)
                }
            }
        },
        $ON_EXEC_ON_READY_FUNCTION: function() {
            "function" == typeof this.oApp.htRunOptions.fnOnAppReady && this.oApp.htRunOptions.fnOnAppReady()
        }
    })
}, function(t, e) {
    nhn.husky.HuskyRangeManager = jindo.$Class({
        name: "HuskyRangeManager",
        oWindow: null,
        $init: function(t) {
            this.oWindow = t || window
        },
        $BEFORE_MSG_APP_READY: function() {
            this.oWindow && "IFRAME" == this.oWindow.tagName && (this.oWindow = this.oWindow.contentWindow, nhn.CurrentSelection.setWindow(this.oWindow)), this.oApp.exec("ADD_APP_PROPERTY", ["getSelection", jindo.$Fn(this.getSelection, this).bind()]), this.oApp.exec("ADD_APP_PROPERTY", ["getEmptySelection", jindo.$Fn(this.getEmptySelection, this).bind()])
        },
        $ON_SET_EDITING_WINDOW: function(t) {
            this.oWindow = t
        },
        getEmptySelection: function(t) {
            return new nhn.HuskyRange(t || this.oWindow)
        },
        getSelection: function(t) {
            this.oApp.exec("RESTORE_IE_SELECTION", []);
            var e = this.getEmptySelection(t);
            try {
                e.setFromSelection()
            } catch (n) {}
            return e
        }
    })
}]);