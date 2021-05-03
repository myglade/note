/*! Copyright (C) NAVER corp. Licensed under LGPL v2. @see https://github.com/naver/smarteditor2/blob/master/LICENSE.md */ ! function(i) {
    var s = {};

    function o(e) {
        if (s[e]) return s[e].exports;
        var t = s[e] = {
            i: e,
            l: !1,
            exports: {}
        };
        return i[e].call(t.exports, t, t.exports, o), t.l = !0, t.exports
    }
    o.m = i, o.c = s, o.d = function(e, t, i) {
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
            for (var s in t) o.d(i, s, function(e) {
                return t[e]
            }.bind(null, s));
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
    }, o.p = "", o(o.s = 56)
}({
    56: function(e, t, i) {
        "use strict";
        i(57), i(58), i(59), i(60), i(61), i(62), i(63), i(64), i(65), i(66), i(67), i(68), i(69), i(70)
    },
    57: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_SE2M_FindReplacePlugin$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.SE2M_FindReplacePlugin, {
            _assignHTMLElements: function() {
                var e = this.oApp.htOptions.elAppContainer;
                this.oApp.exec("LOAD_HTML", ["find_and_replace"]), this.elDropdownLayer = jindo.$$.getSingle("DIV.husky_se2m_findAndReplace_layer", e), this.welDropdownLayer = jindo.$Element(this.elDropdownLayer);
                var t = jindo.$$("LI", this.elDropdownLayer);
                this.oFindTab = t[0], this.oReplaceTab = t[1], t = jindo.$$(".container > .bx", this.elDropdownLayer), this.oFindInputSet = jindo.$$.getSingle(".husky_se2m_find_ui", this.elDropdownLayer), this.oReplaceInputSet = jindo.$$.getSingle(".husky_se2m_replace_ui", this.elDropdownLayer), this.elTitle = jindo.$$.getSingle("H3", this.elDropdownLayer), this.oFindInput_Keyword = jindo.$$.getSingle("INPUT", this.oFindInputSet), t = jindo.$$("INPUT", this.oReplaceInputSet), this.oReplaceInput_Original = t[0], this.oReplaceInput_Replacement = t[1], this.oFindNextButton = jindo.$$.getSingle("BUTTON.husky_se2m_find_next", this.elDropdownLayer), this.oReplaceFindNextButton = jindo.$$.getSingle("BUTTON.husky_se2m_replace_find_next", this.elDropdownLayer), this.oReplaceButton = jindo.$$.getSingle("BUTTON.husky_se2m_replace", this.elDropdownLayer), this.oReplaceAllButton = jindo.$$.getSingle("BUTTON.husky_se2m_replace_all", this.elDropdownLayer), this.aCloseButtons = jindo.$$("BUTTON.husky_se2m_cancel", this.elDropdownLayer)
            },
            $LOCAL_BEFORE_FIRST: function() {
                this._assignHTMLElements(), this.oFindReplace = new nhn.FindReplace(this.oEditingWindow);
                for (var e = 0; e < this.aCloseButtons.length; e++) {
                    var t = jindo.$Fn(this.oApp.exec, this.oApp).bind("HIDE_FIND_REPLACE_LAYER", [this.elDropdownLayer]);
                    jindo.$Fn(t, this).attach(this.aCloseButtons[e], "click")
                }
                jindo.$Fn(jindo.$Fn(this.oApp.exec, this.oApp).bind("SHOW_FIND", []), this).attach(this.oFindTab, "click"), jindo.$Fn(jindo.$Fn(this.oApp.exec, this.oApp).bind("SHOW_REPLACE", []), this).attach(this.oReplaceTab, "click"), jindo.$Fn(jindo.$Fn(this.oApp.exec, this.oApp).bind("FIND", []), this).attach(this.oFindNextButton, "click"), jindo.$Fn(jindo.$Fn(this.oApp.exec, this.oApp).bind("FIND", []), this).attach(this.oReplaceFindNextButton, "click"), jindo.$Fn(jindo.$Fn(this.oApp.exec, this.oApp).bind("REPLACE", []), this).attach(this.oReplaceButton, "click"), jindo.$Fn(jindo.$Fn(this.oApp.exec, this.oApp).bind("REPLACE_ALL", []), this).attach(this.oReplaceAllButton, "click"), this.oFindInput_Keyword.value = "", this.oReplaceInput_Original.value = "", this.oReplaceInput_Replacement.value = "";
                var i = this.oApp.getWYSIWYGWindow().frameElement;
                this.htOffsetPos = jindo.$Element(i).offset(), this.nEditorWidth = i.offsetWidth, this.elDropdownLayer.style.display = "block", this.htInitialPos = this.welDropdownLayer.offset(), this.welDropdownLayer.offset(this.htOffsetPos.top, this.htOffsetPos.left), this.htTopLeftCorner = {
                    x: parseInt(this.elDropdownLayer.style.left, 10),
                    y: parseInt(this.elDropdownLayer.style.top, 10)
                }, this.nLayerWidth = 258, this.nLayerHeight = 160, this.elDropdownLayer.style.display = "none"
            },
            $ON_TOGGLE_FIND_REPLACE_LAYER: function() {
                this.bLayerShown ? this.oApp.exec("HIDE_FIND_REPLACE_LAYER") : this.oApp.exec("SHOW_FIND_REPLACE_LAYER")
            },
            $ON_SHOW_FIND_REPLACE_LAYER: function() {
                this.bLayerShown = !0, this.oApp.exec("DISABLE_ALL_UI", [{
                    aExceptions: ["findAndReplace"]
                }]), this.oApp.exec("SELECT_UI", ["findAndReplace"]), this.oApp.exec("HIDE_ALL_DIALOG_LAYER", []), this.elDropdownLayer.style.top = this.nDefaultTop + "px", this.oApp.exec("SHOW_DIALOG_LAYER", [this.elDropdownLayer, {
                    elHandle: this.elTitle,
                    fnOnDragStart: jindo.$Fn(this.oApp.exec, this.oApp).bind("SHOW_EDITING_AREA_COVER"),
                    fnOnDragEnd: jindo.$Fn(this.oApp.exec, this.oApp).bind("HIDE_EDITING_AREA_COVER"),
                    nMinX: this.htTopLeftCorner.x,
                    nMinY: this.nDefaultTop,
                    nMaxX: this.htTopLeftCorner.x + this.oApp.getEditingAreaWidth() - this.nLayerWidth,
                    nMaxY: this.htTopLeftCorner.y + this.oApp.getEditingAreaHeight() - this.nLayerHeight,
                    sOnShowMsg: "FIND_REPLACE_LAYER_SHOWN"
                }]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["findreplace"])
            },
            $ON_HIDE_FIND_REPLACE_LAYER: function() {
                this.oApp.exec("ENABLE_ALL_UI"), this.oApp.exec("DESELECT_UI", ["findAndReplace"]), this.oApp.exec("HIDE_ALL_DIALOG_LAYER", []), this.bLayerShown = !1
            },
            $ON_FIND_REPLACE_LAYER_SHOWN: function() {
                this.oApp.exec("POSITION_TOOLBAR_LAYER", [this.elDropdownLayer]), this.bFindMode ? (this.oFindInput_Keyword.value = "_clear_", this.oFindInput_Keyword.value = "", this.oFindInput_Keyword.focus()) : (this.oReplaceInput_Original.value = "_clear_", this.oReplaceInput_Original.value = "", this.oReplaceInput_Replacement.value = "", this.oReplaceInput_Original.focus()), this.oApp.exec("HIDE_CURRENT_ACTIVE_LAYER", [])
            },
            $ON_SHOW_FIND_LAYER: function() {
                this.oApp.exec("SHOW_FIND"), this.oApp.exec("SHOW_FIND_REPLACE_LAYER")
            },
            $ON_SHOW_REPLACE_LAYER: function() {
                this.oApp.exec("SHOW_REPLACE"), this.oApp.exec("SHOW_FIND_REPLACE_LAYER")
            },
            $ON_SHOW_FIND: function() {
                this.bFindMode = !0, this.oFindInput_Keyword.value = this.oReplaceInput_Original.value, jindo.$Element(this.oFindTab).addClass("active"), jindo.$Element(this.oReplaceTab).removeClass("active"), jindo.$Element(this.oFindNextButton).removeClass("normal"), jindo.$Element(this.oFindNextButton).addClass("strong"), this.oFindInputSet.style.display = "block", this.oReplaceInputSet.style.display = "none", this.oReplaceButton.style.display = "none", this.oReplaceAllButton.style.display = "none", jindo.$Element(this.elDropdownLayer).removeClass("replace"), jindo.$Element(this.elDropdownLayer).addClass("find")
            },
            $ON_SHOW_REPLACE: function() {
                this.bFindMode = !1, this.oReplaceInput_Original.value = this.oFindInput_Keyword.value, jindo.$Element(this.oFindTab).removeClass("active"), jindo.$Element(this.oReplaceTab).addClass("active"), jindo.$Element(this.oFindNextButton).removeClass("strong"), jindo.$Element(this.oFindNextButton).addClass("normal"), this.oFindInputSet.style.display = "none", this.oReplaceInputSet.style.display = "block", this.oReplaceButton.style.display = "inline", this.oReplaceAllButton.style.display = "inline", jindo.$Element(this.elDropdownLayer).removeClass("find"), jindo.$Element(this.elDropdownLayer).addClass("replace")
            },
            $ON_FIND: function() {
                var e;
                e = this.bFindMode ? this.oFindInput_Keyword.value : this.oReplaceInput_Original.value;
                var t = this.oApp.getSelection();
                switch (t.select(), this.oFindReplace.find(e, !1)) {
                    case 1:
                        alert(this.oApp.$MSG("SE_FindReplace.keywordNotFound")), t.select();
                        break;
                    case 2:
                        alert(this.oApp.$MSG("SE_FindReplace.keywordMissing"))
                }
            },
            $ON_REPLACE: function() {
                var e = this.oReplaceInput_Original.value,
                    t = this.oReplaceInput_Replacement.value,
                    i = this.oApp.getSelection();
                this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["REPLACE"]);
                var s = this.oFindReplace.replace(e, t, !1);
                switch (this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["REPLACE"]), s) {
                    case 1:
                    case 3:
                        alert(this.oApp.$MSG("SE_FindReplace.keywordNotFound")), i.select();
                        break;
                    case 4:
                        alert(this.oApp.$MSG("SE_FindReplace.keywordMissing"))
                }
            },
            $ON_REPLACE_ALL: function() {
                var e = this.oReplaceInput_Original.value,
                    t = this.oReplaceInput_Replacement.value,
                    i = this.oApp.getSelection();
                this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["REPLACE ALL", {
                    sSaveTarget: "BODY"
                }]);
                var s = this.oFindReplace.replaceAll(e, t, !1);
                this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["REPLACE ALL", {
                    sSaveTarget: "BODY"
                }]), 0 === s ? (alert(this.oApp.$MSG("SE_FindReplace.replaceKeywordNotFound")), i.select(), this.oApp.exec("FOCUS")) : s < 0 ? (alert(this.oApp.$MSG("SE_FindReplace.keywordMissing")), i.select()) : (alert(this.oApp.$MSG("SE_FindReplace.replaceAllResultP1") + s + this.oApp.$MSG("SE_FindReplace.replaceAllResultP2")), (i = this.oApp.getEmptySelection()).select(), this.oApp.exec("FOCUS"))
            }
        })
    },
    58: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_SE2M_Quote$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.SE2M_Quote, {
            $ON_TOGGLE_BLOCKQUOTE_LAYER: function() {
                this.oApp.exec("TOGGLE_TOOLBAR_ACTIVE_LAYER", [this.elDropdownLayer, null, "SELECT_UI", ["quote"], "DESELECT_UI", ["quote"]]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["quote"])
            },
            $ON_EVENT_SE2_BLOCKQUOTE_LAYER_CLICK: function(e) {
                var t = nhn.husky.SE2M_Utils.findAncestorByTagName("BUTTON", e.element);
                if (t && "BUTTON" == t.tagName) {
                    var i = t.className;
                    this.oApp.exec("APPLY_BLOCKQUOTE", [i])
                }
            },
            $ON_APPLY_BLOCKQUOTE: function(e) {
                e.match(/(se2_quote[0-9]+)/) ? this._wrapBlock("BLOCKQUOTE", RegExp.$1) : this._unwrapBlock("BLOCKQUOTE"), this.oApp.exec("HIDE_ACTIVE_LAYER", [])
            },
            _isExceedMaxDepth: function(e) {
                var o = function(e) {
                    var t = e.firstChild,
                        i = 0,
                        s = 0;
                    if (!t) return e.tagName && "BLOCKQUOTE" === e.tagName ? 1 : 0;
                    for (; t;) {
                        if (1 === t.nodeType && (i = o(t), "BLOCKQUOTE" === t.tagName && (i += 1), s < i && (s = i), s >= this.nMaxLevel)) return s;
                        t = t.nextSibling
                    }
                    return s
                };
                return o(e) >= this.nMaxLevel
            },
            _unwrapBlock: function(e) {
                for (var t = this.oApp.getSelection(), i = t.commonAncestorContainer; i && i.tagName != e;) i = i.parentNode;
                if (i) {
                    this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["CANCEL BLOCK QUOTE", {
                        sSaveTarget: "BODY"
                    }]);
                    var s = t.commonAncestorContainer;
                    if (3 !== s.nodeType) {
                        var o = t.getTextNodes() || "",
                            n = o.length - 1;
                        s = -1 < n ? o[n] : null
                    }
                    for (; i.firstChild;) i.parentNode.insertBefore(i.firstChild, i);
                    i.parentNode.removeChild(i), s && (t.selectNodeContents(s), t.collapseToEnd(), t.select()), this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["CANCEL BLOCK QUOTE", {
                        sSaveTarget: "BODY"
                    }])
                }
            },
            _wrapBlock: function(e, t) {
                var i, s, o, n, l, r, a, h, _, p, d, c, E, T, A, S, u, g, C, f, L = /BODY|TD|LI/i;
                if (this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["BLOCK QUOTE", {
                        sSaveTarget: "BODY"
                    }]), o = (s = (i = this.oApp.getSelection()).startContainer === i.endContainer && 1 === i.startContainer.nodeType && "P" === i.startContainer.tagName && (nhn.husky.SE2M_Utils.isBlankNode(i.startContainer) || nhn.husky.SE2M_Utils.isFirstChildOfNode("IMG", i.startContainer.tagName, i.startContainer) || nhn.husky.SE2M_Utils.isFirstChildOfNode("IFRAME", i.startContainer.tagName, i.startContainer)) ? i.getLineInfo(!0) : i.getLineInfo(!1)).oStart, n = s.oEnd, l = o.bParentBreak && !L.test(o.oLineBreaker.tagName) ? o.oNode.parentNode : o.oNode, r = n.bParentBreak && !L.test(n.oLineBreaker.tagName) ? n.oNode.parentNode : n.oNode, i.setStartBefore(l), i.setEndAfter(r), (a = this._expandToTableStart(i, r)) && (r = a, i.setEndAfter(a)), (a = this._expandToTableStart(i, l)) && (l = a, i.setStartBefore(a)), a = l, i.fixCommonAncestorContainer(), h = i.commonAncestorContainer, _ = i.startContainer == i.endContainer && i.endOffset - i.startOffset == 1 ? i.startContainer.childNodes[i.startOffset] : i.commonAncestorContainer, p = this._findParentQuote(_)) return p.className = t, void this._setStyle(p, this.htQuoteStyles_view[t]);
                for (; !h.tagName || h.tagName && h.tagName.match(/UL|OL|LI|IMG|IFRAME/);) h = h.parentNode;
                for (; a && a != h && a.parentNode != h;) a = a.parentNode;
                if (d = a == h ? h.firstChild : a, c = i._document.createElement(e), t && (c.className = t, this._setStyle(c, this.htQuoteStyles_view[t])), h.insertBefore(c, d), i.setStartAfter(c), i.setEndAfter(r), i.surroundContents(c), !this._isExceedMaxDepth(c)) return i.selectNodeContents(c), c && c.parentNode && "BODY" == c.parentNode.tagName && !c.nextSibling && ((C = i._document.createElement("P")).innerHTML = "&nbsp;", c.parentNode.insertBefore(C, c.nextSibling)), nhn.husky.SE2M_Utils.isBlankNode(c) && (c.innerHTML = "&nbsp;", i.selectNodeContents(c.firstChild), i.collapseToStart(), i.select()), setTimeout(jindo.$Fn(function(e) {
                    f = e.placeStringBookmark(), e.select(), e.removeStringBookmark(f), this.oApp.exec("FOCUS")
                }, this).bind(i), 0), this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["BLOCK QUOTE", {
                    sSaveTarget: "BODY"
                }]), c;
                for (alert(this.oApp.$MSG("SE2M_Quote.exceedMaxCount").replace("#MaxCount#", this.nMaxLevel + 1)), this.oApp.exec("HIDE_ACTIVE_LAYER", []), E = c.nextSibling, T = c.parentNode, A = c.childNodes, S = [], jindo.$Element(c).leave(), u = 0, g = A.length; u < g; u++) S[u] = A[u];
                for (u = 0, g = S.length; u < g; u++) E ? jindo.$Element(E).before(S[u]) : jindo.$Element(T).append(S[u])
            },
            _expandToTableStart: function(e, t) {
                for (var i = e.commonAncestorContainer, s = null, o = !1; t && !o;) {
                    if (t == i && (o = !0), /TBODY|TFOOT|THEAD|TR/i.test(t.tagName)) {
                        s = this._getTableRoot(t);
                        break
                    }
                    t = t.parentNode
                }
                return s
            },
            _getTableRoot: function(e) {
                for (; e && "TABLE" != e.tagName;) e = e.parentNode;
                return e
            },
            _setStyle: function(e, t) {
                e.setAttribute("style", t), e.style.cssText = t
            }
        })
    },
    59: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_SE2M_SCharacter$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.SE2M_SCharacter, {
            _assignHTMLObjects: function(e) {
                e = jindo.$(e) || document, this.elDropdownLayer = jindo.$$.getSingle("DIV.husky_seditor_sCharacter_layer", e), this.oTextField = jindo.$$.getSingle("INPUT", this.elDropdownLayer), this.oInsertButton = jindo.$$.getSingle("BUTTON.se2_confirm", this.elDropdownLayer), this.aCloseButton = jindo.$$("BUTTON.husky_se2m_sCharacter_close", this.elDropdownLayer), this.aSCharList = jindo.$$("UL.husky_se2m_sCharacter_list", this.elDropdownLayer);
                var t = jindo.$$.getSingle("UL.se2_char_tab", this.elDropdownLayer);
                this.aLabel = jindo.$$(">LI", t)
            },
            $LOCAL_BEFORE_FIRST: function() {
                this.bIE = jindo.$Agent().navigator().ie, this._assignHTMLObjects(this.oApp.htOptions.elAppContainer), this.charSet = [], this.charSet[0] = unescape("FF5B FF5D 3014 3015 3008 3009 300A 300B 300C 300D 300E 300F 3010 3011 2018 2019 201C 201D 3001 3002 %B7 2025 2026 %A7 203B 2606 2605 25CB 25CF 25CE 25C7 25C6 25A1 25A0 25B3 25B2 25BD 25BC 25C1 25C0 25B7 25B6 2664 2660 2661 2665 2667 2663 2299 25C8 25A3 25D0 25D1 2592 25A4 25A5 25A8 25A7 25A6 25A9 %B1 %D7 %F7 2260 2264 2265 221E 2234 %B0 2032 2033 2220 22A5 2312 2202 2261 2252 226A 226B 221A 223D 221D 2235 222B 222C 2208 220B 2286 2287 2282 2283 222A 2229 2227 2228 FFE2 21D2 21D4 2200 2203 %B4 FF5E 02C7 02D8 02DD 02DA 02D9 %B8 02DB %A1 %BF 02D0 222E 2211 220F 266D 2669 266A 266C 327F 2192 2190 2191 2193 2194 2195 2197 2199 2196 2198 321C 2116 33C7 2122 33C2 33D8 2121 2668 260F 260E 261C 261E %B6 2020 2021 %AE %AA %BA 2642 2640").replace(/(\S{4})/g, function(e) {
                    return "%u" + e
                }).split(" "), this.charSet[1] = unescape("%BD 2153 2154 %BC %BE 215B 215C 215D 215E %B9 %B2 %B3 2074 207F 2081 2082 2083 2084 2160 2161 2162 2163 2164 2165 2166 2167 2168 2169 2170 2171 2172 2173 2174 2175 2176 2177 2178 2179 FFE6 %24 FFE5 FFE1 20AC 2103 212B 2109 FFE0 %A4 2030 3395 3396 3397 2113 3398 33C4 33A3 33A4 33A5 33A6 3399 339A 339B 339C 339D 339E 339F 33A0 33A1 33A2 33CA 338D 338E 338F 33CF 3388 3389 33C8 33A7 33A8 33B0 33B1 33B2 33B3 33B4 33B5 33B6 33B7 33B8 33B9 3380 3381 3382 3383 3384 33BA 33BB 33BC 33BD 33BE 33BF 3390 3391 3392 3393 3394 2126 33C0 33C1 338A 338B 338C 33D6 33C5 33AD 33AE 33AF 33DB 33A9 33AA 33AB 33AC 33DD 33D0 33D3 33C3 33C9 33DC 33C6").replace(/(\S{4})/g, function(e) {
                    return "%u" + e
                }).split(" "), this.charSet[2] = unescape("3260 3261 3262 3263 3264 3265 3266 3267 3268 3269 326A 326B 326C 326D 326E 326F 3270 3271 3272 3273 3274 3275 3276 3277 3278 3279 327A 327B 24D0 24D1 24D2 24D3 24D4 24D5 24D6 24D7 24D8 24D9 24DA 24DB 24DC 24DD 24DE 24DF 24E0 24E1 24E2 24E3 24E4 24E5 24E6 24E7 24E8 24E9 2460 2461 2462 2463 2464 2465 2466 2467 2468 2469 246A 246B 246C 246D 246E 3200 3201 3202 3203 3204 3205 3206 3207 3208 3209 320A 320B 320C 320D 320E 320F 3210 3211 3212 3213 3214 3215 3216 3217 3218 3219 321A 321B 249C 249D 249E 249F 24A0 24A1 24A2 24A3 24A4 24A5 24A6 24A7 24A8 24A9 24AA 24AB 24AC 24AD 24AE 24AF 24B0 24B1 24B2 24B3 24B4 24B5 2474 2475 2476 2477 2478 2479 247A 247B 247C 247D 247E 247F 2480 2481 2482").replace(/(\S{4})/g, function(e) {
                    return "%u" + e
                }).split(" "), this.charSet[3] = unescape("3131 3132 3133 3134 3135 3136 3137 3138 3139 313A 313B 313C 313D 313E 313F 3140 3141 3142 3143 3144 3145 3146 3147 3148 3149 314A 314B 314C 314D 314E 314F 3150 3151 3152 3153 3154 3155 3156 3157 3158 3159 315A 315B 315C 315D 315E 315F 3160 3161 3162 3163 3165 3166 3167 3168 3169 316A 316B 316C 316D 316E 316F 3170 3171 3172 3173 3174 3175 3176 3177 3178 3179 317A 317B 317C 317D 317E 317F 3180 3181 3182 3183 3184 3185 3186 3187 3188 3189 318A 318B 318C 318D 318E").replace(/(\S{4})/g, function(e) {
                    return "%u" + e
                }).split(" "), this.charSet[4] = unescape("0391 0392 0393 0394 0395 0396 0397 0398 0399 039A 039B 039C 039D 039E 039F 03A0 03A1 03A3 03A4 03A5 03A6 03A7 03A8 03A9 03B1 03B2 03B3 03B4 03B5 03B6 03B7 03B8 03B9 03BA 03BB 03BC 03BD 03BE 03BF 03C0 03C1 03C3 03C4 03C5 03C6 03C7 03C8 03C9 %C6 %D0 0126 0132 013F 0141 %D8 0152 %DE 0166 014A %E6 0111 %F0 0127 I 0133 0138 0140 0142 0142 0153 %DF %FE 0167 014B 0149 0411 0413 0414 0401 0416 0417 0418 0419 041B 041F 0426 0427 0428 0429 042A 042B 042C 042D 042E 042F 0431 0432 0433 0434 0451 0436 0437 0438 0439 043B 043F 0444 0446 0447 0448 0449 044A 044B 044C 044D 044E 044F").replace(/(\S{4})/g, function(e) {
                    return "%u" + e
                }).split(" "), this.charSet[5] = unescape("3041 3042 3043 3044 3045 3046 3047 3048 3049 304A 304B 304C 304D 304E 304F 3050 3051 3052 3053 3054 3055 3056 3057 3058 3059 305A 305B 305C 305D 305E 305F 3060 3061 3062 3063 3064 3065 3066 3067 3068 3069 306A 306B 306C 306D 306E 306F 3070 3071 3072 3073 3074 3075 3076 3077 3078 3079 307A 307B 307C 307D 307E 307F 3080 3081 3082 3083 3084 3085 3086 3087 3088 3089 308A 308B 308C 308D 308E 308F 3090 3091 3092 3093 30A1 30A2 30A3 30A4 30A5 30A6 30A7 30A8 30A9 30AA 30AB 30AC 30AD 30AE 30AF 30B0 30B1 30B2 30B3 30B4 30B5 30B6 30B7 30B8 30B9 30BA 30BB 30BC 30BD 30BE 30BF 30C0 30C1 30C2 30C3 30C4 30C5 30C6 30C7 30C8 30C9 30CA 30CB 30CC 30CD 30CE 30CF 30D0 30D1 30D2 30D3 30D4 30D5 30D6 30D7 30D8 30D9 30DA 30DB 30DC 30DD 30DE 30DF 30E0 30E1 30E2 30E3 30E4 30E5 30E6 30E7 30E8 30E9 30EA 30EB 30EC 30ED 30EE 30EF 30F0 30F1 30F2 30F3 30F4 30F5 30F6").replace(/(\S{4})/g, function(e) {
                    return "%u" + e
                }).split(" ");
                var e = jindo.$Fn(this.oApp.exec, this.oApp).bind("INSERT_SCHARACTERS", [this.oTextField.value]);
                jindo.$Fn(e, this).attach(this.oInsertButton, "click"), this.oApp.exec("SET_SCHARACTER_LIST", [this.charSet]);
                for (var t = 0; t < this.aLabel.length; t++) {
                    var i = jindo.$Fn(this.oApp.exec, this.oApp).bind("CHANGE_SCHARACTER_SET", [t]);
                    jindo.$Fn(i, this).attach(this.aLabel[t].firstChild, "mousedown")
                }
                for (t = 0; t < this.aCloseButton.length; t++) this.oApp.registerBrowserEvent(this.aCloseButton[t], "click", "HIDE_ACTIVE_LAYER", []);
                this.oApp.registerBrowserEvent(this.elDropdownLayer, "click", "EVENT_SCHARACTER_CLICKED", []), this.oApp.registerBrowserEvent(this.oTextField, "keydown", "EVENT_SCHARACTER_KEYDOWN")
            },
            $ON_EVENT_SCHARACTER_KEYDOWN: function(e) {
                e.key().enter && (this.oApp.exec("INSERT_SCHARACTERS"), e.stop())
            },
            $ON_TOGGLE_SCHARACTER_LAYER: function() {
                this.oTextField.value = "", this.oSelection = this.oApp.getSelection(), this.oApp.exec("TOGGLE_TOOLBAR_ACTIVE_LAYER", [this.elDropdownLayer, null, "MSG_SCHARACTER_LAYER_SHOWN", [], "MSG_SCHARACTER_LAYER_HIDDEN", [""]]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["symbol"])
            },
            $ON_MSG_SCHARACTER_LAYER_SHOWN: function() {
                this.oTextField.focus(), this.oApp.exec("SELECT_UI", ["sCharacter"])
            },
            $ON_MSG_SCHARACTER_LAYER_HIDDEN: function() {
                this.oApp.exec("DESELECT_UI", ["sCharacter"])
            },
            $ON_EVENT_SCHARACTER_CLICKED: function(e) {
                var t = nhn.husky.SE2M_Utils.findAncestorByTagName("BUTTON", e.element);
                if (t && "BUTTON" == t.tagName && "LI" == t.parentNode.tagName) {
                    var i = t.firstChild.innerHTML;
                    1 < i.length || (this.oApp.exec("SELECT_SCHARACTER", [i]), e.stop())
                }
            },
            $ON_SELECT_SCHARACTER: function(e) {
                if (this.oTextField.value += e, this.oTextField.createTextRange) {
                    var t = this.oTextField.createTextRange();
                    t.collapse(!1), t.select()
                } else this.oTextField.selectionEnd && (this.oTextField.selectionEnd = this.oTextField.value.length, this.oTextField.focus())
            },
            $ON_INSERT_SCHARACTERS: function() {
                this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["INSERT SCHARACTER"]), this.oApp.exec("PASTE_HTML", [this.oTextField.value]), this.oApp.exec("FOCUS"), this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["INSERT SCHARACTER"]), this.oApp.exec("HIDE_ACTIVE_LAYER", [])
            },
            $ON_CHANGE_SCHARACTER_SET: function(e) {
                for (var t = 0; t < this.aSCharList.length; t++)
                    if (jindo.$Element(this.aLabel[t]).hasClass("active")) {
                        if (t == e) return;
                        jindo.$Element(this.aLabel[t]).removeClass("active")
                    } this._drawSCharList(e), jindo.$Element(this.aLabel[e]).addClass("active")
            },
            $ON_SET_SCHARACTER_LIST: function(e) {
                this.charSet = e, this.bSCharSetDrawn = new Array(this.charSet.length), this._drawSCharList(0)
            },
            _drawSCharList: function(e) {
                if (!this.bSCharSetDrawn[e]) {
                    this.bSCharSetDrawn[e] = !0;
                    var t, i, s = this.charSet[e].length,
                        o = new Array(s);
                    this.aSCharList[e].innerHTML = "";
                    for (var n = 0; n < s; n++) o[n] = jindo.$("<LI>"), this.bIE ? (t = jindo.$("<BUTTON>")).setAttribute("type", "button") : (t = jindo.$("<BUTTON>")).type = "button", (i = jindo.$("<SPAN>")).innerHTML = unescape(this.charSet[e][n]), t.appendChild(i), o[n].appendChild(t), o[n].onmouseover = function() {
                        this.className = "hover"
                    }, o[n].onmouseout = function() {
                        this.className = ""
                    }, this.aSCharList[e].appendChild(o[n])
                }
            }
        })
    },
    60: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_SE2M_TableCreator$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.SE2M_TableCreator, {
            _assignHTMLObjects: function(e) {
                this.oApp.exec("LOAD_HTML", ["create_table"]);
                var t = null;
                this.elDropdownLayer = jindo.$$.getSingle("DIV.husky_se2m_table_layer", e), this.welDropdownLayer = jindo.$Element(this.elDropdownLayer), t = jindo.$$("INPUT", this.elDropdownLayer), this.elText_row = t[0], this.elText_col = t[1], this.elRadio_manualStyle = t[2], this.elText_borderSize = t[3], this.elText_borderColor = t[4], this.elText_BGColor = t[5], this.elRadio_templateStyle = t[6], t = jindo.$$("BUTTON", this.elDropdownLayer), this.elBtn_rowInc = t[0], this.elBtn_rowDec = t[1], this.elBtn_colInc = t[2], this.elBtn_colDec = t[3], this.elBtn_borderStyle = t[4], this.elBtn_incBorderSize = jindo.$$.getSingle("BUTTON.se2m_incBorder", this.elDropdownLayer), this.elBtn_decBorderSize = jindo.$$.getSingle("BUTTON.se2m_decBorder", this.elDropdownLayer), this.elLayer_Dim1 = jindo.$$.getSingle("DIV.se2_t_dim0", this.elDropdownLayer), this.elLayer_Dim2 = jindo.$$.getSingle("DIV.se2_t_dim3", this.elDropdownLayer), t = jindo.$$("SPAN.se2_pre_color>BUTTON", this.elDropdownLayer), this.elBtn_borderColor = t[0], this.elBtn_BGColor = t[1], this.elBtn_tableStyle = jindo.$$.getSingle("DIV.se2_select_ty2>BUTTON", this.elDropdownLayer), t = jindo.$$("P.se2_btn_area>BUTTON", this.elDropdownLayer), this.elBtn_apply = t[0], this.elBtn_cancel = t[1], this.elTable_preview = jindo.$$.getSingle("TABLE.husky_se2m_table_preview", this.elDropdownLayer), this.elLayer_borderStyle = jindo.$$.getSingle("DIV.husky_se2m_table_border_style_layer", this.elDropdownLayer), this.elPanel_borderStylePreview = jindo.$$.getSingle("SPAN.husky_se2m_table_border_style_preview", this.elDropdownLayer), this.elPanel_borderColorPallet = jindo.$$.getSingle("DIV.husky_se2m_table_border_color_pallet", this.elDropdownLayer), this.elPanel_bgColorPallet = jindo.$$.getSingle("DIV.husky_se2m_table_bgcolor_pallet", this.elDropdownLayer), this.elLayer_tableStyle = jindo.$$.getSingle("DIV.husky_se2m_table_style_layer", this.elDropdownLayer), this.elPanel_tableStylePreview = jindo.$$.getSingle("SPAN.husky_se2m_table_style_preview", this.elDropdownLayer), this.aElBtn_borderStyle = jindo.$$("BUTTON", this.elLayer_borderStyle), this.aElBtn_tableStyle = jindo.$$("BUTTON", this.elLayer_tableStyle), this.sNoBorderText = jindo.$$.getSingle("SPAN.se2m_no_border", this.elDropdownLayer).innerHTML, this.rxLastDigits = RegExp("([0-9]+)$")
            },
            $LOCAL_BEFORE_FIRST: function() {
                var e;
                for (this._assignHTMLObjects(this.oApp.htOptions.elAppContainer), this.oApp.registerBrowserEvent(this.elText_row, "change", "TABLE_SET_ROW_NUM", [null, 0]), this.oApp.registerBrowserEvent(this.elText_col, "change", "TABLE_SET_COLUMN_NUM", [null, 0]), this.oApp.registerBrowserEvent(this.elText_borderSize, "change", "TABLE_SET_BORDER_SIZE", [null, 0]), this.oApp.registerBrowserEvent(this.elBtn_rowInc, "click", "TABLE_INC_ROW"), this.oApp.registerBrowserEvent(this.elBtn_rowDec, "click", "TABLE_DEC_ROW"), jindo.$Fn(this._numRowKeydown, this).attach(this.elText_row.parentNode, "keydown"), this.oApp.registerBrowserEvent(this.elBtn_colInc, "click", "TABLE_INC_COLUMN"), this.oApp.registerBrowserEvent(this.elBtn_colDec, "click", "TABLE_DEC_COLUMN"), jindo.$Fn(this._numColKeydown, this).attach(this.elText_col.parentNode, "keydown"), this.oApp.registerBrowserEvent(this.elBtn_incBorderSize, "click", "TABLE_INC_BORDER_SIZE"), this.oApp.registerBrowserEvent(this.elBtn_decBorderSize, "click", "TABLE_DEC_BORDER_SIZE"), jindo.$Fn(this._borderSizeKeydown, this).attach(this.elText_borderSize.parentNode, "keydown"), this.oApp.registerBrowserEvent(this.elBtn_borderStyle, "click", "TABLE_TOGGLE_BORDER_STYLE_LAYER"), this.oApp.registerBrowserEvent(this.elBtn_tableStyle, "click", "TABLE_TOGGLE_STYLE_LAYER"), this.oApp.registerBrowserEvent(this.elBtn_borderColor, "click", "TABLE_TOGGLE_BORDER_COLOR_PALLET"), this.oApp.registerBrowserEvent(this.elBtn_BGColor, "click", "TABLE_TOGGLE_BGCOLOR_PALLET"), this.oApp.registerBrowserEvent(this.elRadio_manualStyle, "click", "TABLE_ENABLE_MANUAL_STYLE"), this.oApp.registerBrowserEvent(this.elRadio_templateStyle, "click", "TABLE_ENABLE_TEMPLATE_STYLE"), this.oApp.exec("SE2_ATTACH_HOVER_EVENTS", [this.aElBtn_borderStyle]), this.oApp.exec("SE2_ATTACH_HOVER_EVENTS", [this.aElBtn_tableStyle]), e = 0; e < this.aElBtn_borderStyle.length; e++) this.oApp.registerBrowserEvent(this.aElBtn_borderStyle[e], "click", "TABLE_SELECT_BORDER_STYLE");
                for (e = 0; e < this.aElBtn_tableStyle.length; e++) this.oApp.registerBrowserEvent(this.aElBtn_tableStyle[e], "click", "TABLE_SELECT_STYLE");
                this.oApp.registerBrowserEvent(this.elBtn_apply, "click", "TABLE_INSERT"), this.oApp.registerBrowserEvent(this.elBtn_cancel, "click", "HIDE_ACTIVE_LAYER"), this.oApp.exec("TABLE_SET_BORDER_COLOR", [/#[0-9A-Fa-f]{6}/.test(this.elText_borderColor.value) ? this.elText_borderColor.value : "#cccccc"]), this.oApp.exec("TABLE_SET_BGCOLOR", [/#[0-9A-Fa-f]{6}/.test(this.elText_BGColor.value) ? this.elText_BGColor.value : "#ffffff"]), this.nStyleMode = 1, this.aTableStyleByBorder = ["", 'border="1" cellpadding="0" cellspacing="0" style="border:1px dashed #c7c7c7; border-left:0; border-bottom:0;"', 'border="1" cellpadding="0" cellspacing="0" style="border:#BorderSize#px dashed #BorderColor#; border-left:0; border-bottom:0;"', 'border="0" cellpadding="0" cellspacing="0" style="border:#BorderSize#px solid #BorderColor#; border-left:0; border-bottom:0;"', 'border="0" cellpadding="0" cellspacing="1" style="border:#BorderSize#px solid #BorderColor#;"', 'border="0" cellpadding="0" cellspacing="1" style="border:#BorderSize#px double #BorderColor#;"', 'border="0" cellpadding="0" cellspacing="1" style="border-width:#BorderSize*2#px #BorderSize#px #BorderSize#px #BorderSize*2#px; border-style:solid;border-color:#BorderColor#;"', 'border="0" cellpadding="0" cellspacing="1" style="border-width:#BorderSize#px #BorderSize*2#px #BorderSize*2#px #BorderSize#px; border-style:solid;border-color:#BorderColor#;"'], this.aTDStyleByBorder = ["", 'style="border:1px dashed #c7c7c7; border-top:0; border-right:0; background-color:#BGColor#"', 'style="border:#BorderSize#px dashed #BorderColor#; border-top:0; border-right:0; background-color:#BGColor#"', 'style="border:#BorderSize#px solid #BorderColor#; border-top:0; border-right:0; background-color:#BGColor#"', 'style="border:#BorderSize#px solid #BorderColor#; background-color:#BGColor#"', 'style="border:#BorderSize+2#px double #BorderColor#; background-color:#BGColor#"', 'style="border-width:#BorderSize#px #BorderSize*2#px #BorderSize*2#px #BorderSize#px; border-style:solid;border-color:#BorderColor#; background-color:#BGColor#"', 'style="border-width:#BorderSize*2#px #BorderSize#px #BorderSize#px #BorderSize*2#px; border-style:solid;border-color:#BorderColor#; background-color:#BGColor#"'], this.oApp.registerBrowserEvent(this.elDropdownLayer, "keydown", "EVENT_TABLE_CREATE_KEYDOWN"), this._drawTableDropdownLayer()
            },
            $ON_TABLE_SELECT_BORDER_STYLE: function(e) {
                var t = e.currentElement,
                    i = this.rxLastDigits.exec(t.className);
                this._selectBorderStyle(i[1])
            },
            $ON_TABLE_SELECT_STYLE: function(e) {
                var t = this.rxLastDigits.exec(e.element.className);
                this._selectTableStyle(t[1])
            },
            $ON_TOGGLE_TABLE_LAYER: function() {
                this._showNewTable(), this.oApp.exec("TOGGLE_TOOLBAR_ACTIVE_LAYER", [this.elDropdownLayer, null, "SELECT_UI", ["table"], "TABLE_CLOSE", []]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["table"])
            },
            $ON_TABLE_CLOSE_ALL: function() {
                this.oApp.exec("TABLE_HIDE_BORDER_COLOR_PALLET", []), this.oApp.exec("TABLE_HIDE_BGCOLOR_PALLET", []), this.oApp.exec("TABLE_HIDE_BORDER_STYLE_LAYER", []), this.oApp.exec("TABLE_HIDE_STYLE_LAYER", [])
            },
            $ON_TABLE_INC_ROW: function() {
                this.oApp.exec("TABLE_SET_ROW_NUM", [null, 1])
            },
            $ON_TABLE_DEC_ROW: function() {
                this.oApp.exec("TABLE_SET_ROW_NUM", [null, -1])
            },
            $ON_TABLE_INC_COLUMN: function() {
                this.oApp.exec("TABLE_SET_COLUMN_NUM", [null, 1])
            },
            $ON_TABLE_DEC_COLUMN: function() {
                this.oApp.exec("TABLE_SET_COLUMN_NUM", [null, -1])
            },
            $ON_TABLE_SET_ROW_NUM: function(e, t) {
                e = e || parseInt(this.elText_row.value, 10) || 0, (e += t = t || 0) < this.nMinRows && (e = this.nMinRows), e > this.nMaxRows && (e = this.nMaxRows), this.elText_row.value = e, this._showNewTable()
            },
            $ON_TABLE_SET_COLUMN_NUM: function(e, t) {
                e = e || parseInt(this.elText_col.value, 10) || 0, (e += t = t || 0) < this.nMinColumns && (e = this.nMinColumns), e > this.nMaxColumns && (e = this.nMaxColumns), this.elText_col.value = e, this._showNewTable()
            },
            _getTableString: function() {
                return 1 == this.nStyleMode ? this._doGetTableString(this.nColumns, this.nRows, this.nBorderSize, this.sBorderColor, this.sBGColor, this.nBorderStyleIdx) : this._doGetTableString(this.nColumns, this.nRows, this.nBorderSize, this.sBorderColor, this.sBGColor, 0)
            },
            $ON_TABLE_INSERT: function() {
                var e, t, i, s, o, n, l, r, a;
                this.oApp.exec("IE_FOCUS", []), this.oApp.exec("TABLE_SET_COLUMN_NUM"), this.oApp.exec("TABLE_SET_ROW_NUM"), this._loadValuesFromHTML(), i = this.oApp.getWYSIWYGDocument().body, a = jindo.$Agent().navigator(), this.nTableWidth = i.offsetWidth, e = this._getTableString(), (s = this.oApp.getWYSIWYGDocument().createElement("DIV")).innerHTML = e, (o = s.firstChild).className = this._sSETblClass, l = this.oApp.getSelection(), l = this._divideParagraph(l), this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["INSERT TABLE", {
                    sSaveTarget: "BODY"
                }]), r = this.oApp.getWYSIWYGDocument().createElement("DIV"), l.deleteContents(), l.insertNode(r), l.selectNode(r), this.oApp.exec("REMOVE_STYLE", [l]), a.ie && 1 === this.oApp.getWYSIWYGDocument().body.childNodes.length && this.oApp.getWYSIWYGDocument().body.firstChild === r ? r.insertBefore(o, null) : (r.parentNode.insertBefore(o, r), r.parentNode.removeChild(r)), a.firefox ? (t = this.oApp.getWYSIWYGDocument().createElement("BR"), o.parentNode.insertBefore(t, o.nextSibling)) : a.ie && (t = this.oApp.getWYSIWYGDocument().createElement("p"), 11 == document.documentMode && (t.innerHTML = "<br>"), o.parentNode.insertBefore(t, o.nextSibling)), 2 == this.nStyleMode && this.oApp.exec("STYLE_TABLE", [o, this.nTableStyleIdx]), n = o.getElementsByTagName("TD")[0], l.selectNodeContents(n.firstChild || n), l.collapseToEnd(), l.select(), this.oApp.exec("FOCUS"), this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["INSERT TABLE", {
                    sSaveTarget: "BODY"
                }]), this.oApp.exec("HIDE_ACTIVE_LAYER", []), this.oApp.exec("MSG_DISPLAY_REEDIT_MESSAGE_SHOW", [this.name, this.sReEditGuideMsg_table])
            },
            _divideParagraph: function(e) {
                var t, i, s, o, n;
                e.fixCommonAncestorContainer();
                var l = e.commonAncestorContainer,
                    r = nhn.husky.SE2M_Utils.findAncestorByTagNameWithCount("P", l),
                    a = r.elNode;
                if (a) {
                    var h = nhn.husky.SE2M_Utils.findClosestAncestorAmongTagNamesWithCount(["TD", "TH"], l);
                    if (h.elNode) r.nRecursiveCount < h.nRecursiveCount && (t = a);
                    else t = a
                }
                return t && (!t.firstChild || nhn.husky.SE2M_Utils.isBlankNode(t) ? e.selectNode(t) : (s = e.placeStringBookmark(), e.moveToBookmark(s), o = this.oApp.getWYSIWYGDocument().createElement("P"), e.setStartBefore(t.firstChild), e.surroundContents(o), e.collapseToEnd(), n = this.oApp.getWYSIWYGDocument().createElement("P"), e.setEndAfter(t.lastChild), e.surroundContents(n), e.collapseToStart(), e.removeStringBookmark(s), (i = jindo.$Element(t)).after(n), i.after(o), i.leave(), (e = this.oApp.getEmptySelection()).setEndAfter(o), e.setStartBefore(n)), e.select()), e
            },
            $ON_TABLE_CLOSE: function() {
                this.oApp.exec("TABLE_CLOSE_ALL", []), this.oApp.exec("DESELECT_UI", ["table"])
            },
            $ON_TABLE_SET_BORDER_SIZE: function(e, t) {
                e = e || parseInt(this.elText_borderSize.value, 10) || 0, (e += t = t || 0) < this.nMinBorderWidth && (e = this.nMinBorderWidth), e > this.nMaxBorderWidth && (e = this.nMaxBorderWidth), this.elText_borderSize.value = e
            },
            $ON_TABLE_INC_BORDER_SIZE: function() {
                this.oApp.exec("TABLE_SET_BORDER_SIZE", [null, 1])
            },
            $ON_TABLE_DEC_BORDER_SIZE: function() {
                this.oApp.exec("TABLE_SET_BORDER_SIZE", [null, -1])
            },
            $ON_TABLE_TOGGLE_BORDER_STYLE_LAYER: function() {
                "block" == this.elLayer_borderStyle.style.display ? this.oApp.exec("TABLE_HIDE_BORDER_STYLE_LAYER", []) : this.oApp.exec("TABLE_SHOW_BORDER_STYLE_LAYER", [])
            },
            $ON_TABLE_SHOW_BORDER_STYLE_LAYER: function() {
                this.oApp.exec("TABLE_CLOSE_ALL", []), this.elBtn_borderStyle.className = "se2_view_more2", this.elLayer_borderStyle.style.display = "block", this._refresh()
            },
            $ON_TABLE_HIDE_BORDER_STYLE_LAYER: function() {
                this.elBtn_borderStyle.className = "se2_view_more", this.elLayer_borderStyle.style.display = "none", this._refresh()
            },
            $ON_TABLE_TOGGLE_STYLE_LAYER: function() {
                "block" == this.elLayer_tableStyle.style.display ? this.oApp.exec("TABLE_HIDE_STYLE_LAYER", []) : this.oApp.exec("TABLE_SHOW_STYLE_LAYER", [])
            },
            $ON_TABLE_SHOW_STYLE_LAYER: function() {
                this.oApp.exec("TABLE_CLOSE_ALL", []), this.elBtn_tableStyle.className = "se2_view_more2", this.elLayer_tableStyle.style.display = "block", this._refresh()
            },
            $ON_TABLE_HIDE_STYLE_LAYER: function() {
                this.elBtn_tableStyle.className = "se2_view_more", this.elLayer_tableStyle.style.display = "none", this._refresh()
            },
            $ON_TABLE_TOGGLE_BORDER_COLOR_PALLET: function() {
                this.welDropdownLayer.hasClass("p1") ? this.oApp.exec("TABLE_HIDE_BORDER_COLOR_PALLET", []) : this.oApp.exec("TABLE_SHOW_BORDER_COLOR_PALLET", [])
            },
            $ON_TABLE_SHOW_BORDER_COLOR_PALLET: function() {
                this.oApp.exec("TABLE_CLOSE_ALL", []), this.welDropdownLayer.addClass("p1"), this.welDropdownLayer.removeClass("p2"), this.oApp.exec("SHOW_COLOR_PALETTE", ["TABLE_SET_BORDER_COLOR_FROM_PALETTE", this.elPanel_borderColorPallet]), this.elPanel_borderColorPallet.parentNode.style.display = "block"
            },
            $ON_TABLE_HIDE_BORDER_COLOR_PALLET: function() {
                this.welDropdownLayer.removeClass("p1"), this.oApp.exec("HIDE_COLOR_PALETTE", []), this.elPanel_borderColorPallet.parentNode.style.display = "none"
            },
            $ON_TABLE_TOGGLE_BGCOLOR_PALLET: function() {
                this.welDropdownLayer.hasClass("p2") ? this.oApp.exec("TABLE_HIDE_BGCOLOR_PALLET", []) : this.oApp.exec("TABLE_SHOW_BGCOLOR_PALLET", [])
            },
            $ON_TABLE_SHOW_BGCOLOR_PALLET: function() {
                this.oApp.exec("TABLE_CLOSE_ALL", []), this.welDropdownLayer.removeClass("p1"), this.welDropdownLayer.addClass("p2"), this.oApp.exec("SHOW_COLOR_PALETTE", ["TABLE_SET_BGCOLOR_FROM_PALETTE", this.elPanel_bgColorPallet]), this.elPanel_bgColorPallet.parentNode.style.display = "block"
            },
            $ON_TABLE_HIDE_BGCOLOR_PALLET: function() {
                this.welDropdownLayer.removeClass("p2"), this.oApp.exec("HIDE_COLOR_PALETTE", []), this.elPanel_bgColorPallet.parentNode.style.display = "none"
            },
            $ON_TABLE_SET_BORDER_COLOR_FROM_PALETTE: function(e) {
                this.oApp.exec("TABLE_SET_BORDER_COLOR", [e]), this.oApp.exec("TABLE_HIDE_BORDER_COLOR_PALLET", [])
            },
            $ON_TABLE_SET_BORDER_COLOR: function(e) {
                this.elText_borderColor.value = e, this.elBtn_borderColor.style.backgroundColor = e
            },
            $ON_TABLE_SET_BGCOLOR_FROM_PALETTE: function(e) {
                this.oApp.exec("TABLE_SET_BGCOLOR", [e]), this.oApp.exec("TABLE_HIDE_BGCOLOR_PALLET", [])
            },
            $ON_TABLE_SET_BGCOLOR: function(e) {
                this.elText_BGColor.value = e, this.elBtn_BGColor.style.backgroundColor = e
            },
            $ON_TABLE_ENABLE_MANUAL_STYLE: function() {
                this.nStyleMode = 1, this._drawTableDropdownLayer()
            },
            $ON_TABLE_ENABLE_TEMPLATE_STYLE: function() {
                this.nStyleMode = 2, this._drawTableDropdownLayer()
            },
            $ON_EVENT_TABLE_CREATE_KEYDOWN: function(e) {
                e.key().enter && (this.elBtn_apply.focus(), this.oApp.exec("TABLE_INSERT"), e.stop())
            },
            _drawTableDropdownLayer: function() {
                1 == this.nBorderStyleIdx ? (this.elPanel_borderStylePreview.innerHTML = this.sNoBorderText, this.elLayer_Dim1.className = "se2_t_dim2") : (this.elPanel_borderStylePreview.innerHTML = "", this.elLayer_Dim1.className = "se2_t_dim0"), 1 == this.nStyleMode ? (this.elRadio_manualStyle.checked = !0, this.elLayer_Dim2.className = "se2_t_dim3", this.elText_borderSize.disabled = !1, this.elText_borderColor.disabled = !1, this.elText_BGColor.disabled = !1) : (this.elRadio_templateStyle.checked = !0, this.elLayer_Dim2.className = "se2_t_dim1", this.elText_borderSize.disabled = !0, this.elText_borderColor.disabled = !0, this.elText_BGColor.disabled = !0), this.oApp.exec("TABLE_CLOSE_ALL", [])
            },
            _selectBorderStyle: function(e) {
                this.elPanel_borderStylePreview.className = "se2_b_style" + e, this.nBorderStyleIdx = e, this._drawTableDropdownLayer()
            },
            _selectTableStyle: function(e) {
                this.elPanel_tableStylePreview.className = "se2_t_style" + e, this.nTableStyleIdx = e, this._drawTableDropdownLayer()
            },
            _showNewTable: function() {
                var e = document.createElement("DIV");
                this._loadValuesFromHTML(), e.innerHTML = this._getPreviewTableString(this.nColumns, this.nRows);
                var t = e.firstChild;
                this.elTable_preview.parentNode.insertBefore(t, this.elTable_preview), this.elTable_preview.parentNode.removeChild(this.elTable_preview), this.elTable_preview = t, this._refresh()
            },
            _getPreviewTableString: function(e, t) {
                for (var i = '<table border="0" cellspacing="1" class="se2_pre_table husky_se2m_table_preview">', s = "<tr>", o = 0; o < e; o++) s += "<td><p>&nbsp;</p></td>\n";
                for (s += "</tr>\n", i += "<tbody>", o = 0; o < t; o++) i += s;
                return i += "</tbody>\n", i += "</table>\n"
            },
            _loadValuesFromHTML: function() {
                this.nColumns = parseInt(this.elText_col.value, 10) || 1, this.nRows = parseInt(this.elText_row.value, 10) || 1, this.nBorderSize = parseInt(this.elText_borderSize.value, 10) || 1, this.sBorderColor = this.elText_borderColor.value, this.sBGColor = this.elText_BGColor.value
            },
            _doGetTableString: function(e, t, i, s, o, n) {
                var l = parseInt(this.nTableWidth / e, 10),
                    r = this.aTableStyleByBorder[n].replace(/#BorderSize#/g, this.nBorderSize).replace(/#BorderSize\*([0-9]+)#/g, function(e, t) {
                        return i * parseInt(t, 10)
                    }).replace(/#BorderSize\+([0-9]+)#/g, function(e, t) {
                        return i + parseInt(t, 10)
                    }).replace("#BorderColor#", this.sBorderColor).replace("#BGColor#", this.sBGColor),
                    a = this.aTDStyleByBorder[n].replace(/#BorderSize#/g, this.nBorderSize).replace(/#BorderSize\*([0-9]+)#/g, function(e, t) {
                        return i * parseInt(t, 10)
                    }).replace(/#BorderSize\+([0-9]+)#/g, function(e, t) {
                        return i + parseInt(t, 10)
                    }).replace("#BorderColor#", this.sBorderColor).replace("#BGColor#", this.sBGColor);
                l ? a += " width=" + l : r += "class=se2_pre_table";
                for (var h = "<table " + r + " " + (1 == n ? 'attr_no_border_tbl="1"' : "") + ">", _ = "<tr>", p = 0; p < e; p++) _ += "<td " + a + "><p>&nbsp;</p></td>\n";
                for (_ += "</tr>\n", h += "<tbody>\n", p = 0; p < t; p++) h += _;
                return h += "</tbody>\n", h += "</table>\n<br>"
            },
            _numRowKeydown: function(e) {
                var t = e.key();
                38 == t.keyCode && this.oApp.exec("TABLE_INC_ROW", []), 40 == t.keyCode && this.oApp.exec("TABLE_DEC_ROW", [])
            },
            _numColKeydown: function(e) {
                var t = e.key();
                38 == t.keyCode && this.oApp.exec("TABLE_INC_COLUMN", []), 40 == t.keyCode && this.oApp.exec("TABLE_DEC_COLUMN", [])
            },
            _borderSizeKeydown: function(e) {
                var t = e.key();
                38 == t.keyCode && this.oApp.exec("TABLE_INC_BORDER_SIZE", []), 40 == t.keyCode && this.oApp.exec("TABLE_DEC_BORDER_SIZE", [])
            },
            _refresh: function() {
                this.elDropdownLayer.style.zoom = 0, this.elDropdownLayer.style.zoom = ""
            }
        })
    },
    61: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_SE2M_TableEditor$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.SE2M_TableEditor, {
            _aCellName: ["TD", "TH"],
            _assignHTMLObjects: function() {
                this.oApp.exec("LOAD_HTML", ["qe_table"]), this.elQELayer = jindo.$$.getSingle("DIV.q_table_wrap", this.oApp.htOptions.elAppContainer), this.elQELayer.style.zIndex = 150, this.elBtnAddRowBelow = jindo.$$.getSingle("BUTTON.se2_addrow", this.elQELayer), this.elBtnAddColumnRight = jindo.$$.getSingle("BUTTON.se2_addcol", this.elQELayer), this.elBtnSplitRow = jindo.$$.getSingle("BUTTON.se2_seprow", this.elQELayer), this.elBtnSplitColumn = jindo.$$.getSingle("BUTTON.se2_sepcol", this.elQELayer), this.elBtnDeleteRow = jindo.$$.getSingle("BUTTON.se2_delrow", this.elQELayer), this.elBtnDeleteColumn = jindo.$$.getSingle("BUTTON.se2_delcol", this.elQELayer), this.elBtnMergeCell = jindo.$$.getSingle("BUTTON.se2_merrow", this.elQELayer), this.elBtnBGPalette = jindo.$$.getSingle("BUTTON.husky_se2m_table_qe_bgcolor_btn", this.elQELayer), this.elBtnBGIMGPalette = jindo.$$.getSingle("BUTTON.husky_se2m_table_qe_bgimage_btn", this.elQELayer), this.elPanelBGPaletteHolder = jindo.$$.getSingle("DIV.husky_se2m_tbl_qe_bg_paletteHolder", this.elQELayer), this.elPanelBGIMGPaletteHolder = jindo.$$.getSingle("DIV.husky_se2m_tbl_qe_bg_img_paletteHolder", this.elQELayer), this.elPanelTableBGArea = jindo.$$.getSingle("DIV.se2_qe2", this.elQELayer), this.elPanelTableTemplateArea = jindo.$$.getSingle("DL.se2_qe3", this.elQELayer), this.elPanelReviewBGArea = jindo.$$.getSingle("DL.husky_se2m_tbl_qe_review_bg", this.elQELayer), this.elPanelBGImg = jindo.$$.getSingle("DD", this.elPanelReviewBGArea), this.welPanelTableBGArea = jindo.$Element(this.elPanelTableBGArea), this.welPanelTableTemplateArea = jindo.$Element(this.elPanelTableTemplateArea), this.welPanelReviewBGArea = jindo.$Element(this.elPanelReviewBGArea), this.elPanelDim1 = jindo.$$.getSingle("DIV.husky_se2m_tbl_qe_dim1", this.elQELayer), this.elPanelDim2 = jindo.$$.getSingle("DIV.husky_se2m_tbl_qe_dim2", this.elQELayer), this.elPanelDimDelCol = jindo.$$.getSingle("DIV.husky_se2m_tbl_qe_dim_del_col", this.elQELayer), this.elPanelDimDelRow = jindo.$$.getSingle("DIV.husky_se2m_tbl_qe_dim_del_row", this.elQELayer), this.elInputRadioBGColor = jindo.$$.getSingle("INPUT.husky_se2m_radio_bgc", this.elQELayer), this.elInputRadioBGImg = jindo.$$.getSingle("INPUT.husky_se2m_radio_bgimg", this.elQELayer), this.elSelectBoxTemplate = jindo.$$.getSingle("DIV.se2_select_ty2", this.elQELayer), this.elInputRadioTemplate = jindo.$$.getSingle("INPUT.husky_se2m_radio_template", this.elQELayer), this.elPanelQETemplate = jindo.$$.getSingle("DIV.se2_layer_t_style", this.elQELayer), this.elBtnQETemplate = jindo.$$.getSingle("BUTTON.husky_se2m_template_more", this.elQELayer), this.elPanelQETemplatePreview = jindo.$$.getSingle("SPAN.se2_t_style1", this.elQELayer), this.aElBtn_tableStyle = jindo.$$("BUTTON", this.elPanelQETemplate);
                for (var e = 0; e < this.aElBtn_tableStyle.length; e++) this.oApp.registerBrowserEvent(this.aElBtn_tableStyle[e], "click", "TABLE_QE_SELECT_TEMPLATE")
            },
            _attachEvents: function() {
                var e = jindo.$Agent().navigator();
                this.oApp.exec("SE2_ATTACH_HOVER_EVENTS", [this.aElBtn_tableStyle]), this._wfnOnMouseDownResizeCover = jindo.$Fn(this._fnOnMouseDownResizeCover, this), this._wfnOnMouseMoveResizeCover = jindo.$Fn(this._fnOnMouseMoveResizeCover, this), this._wfnOnMouseUpResizeCover = jindo.$Fn(this._fnOnMouseUpResizeCover, this), this._wfnOnMouseDownResizeCover.attach(this.elResizeCover, "mousedown"), e.ie && 8 < e.version && (this._wfnOnResizeEndTable = jindo.$Fn(this._fnOnResizeEndTable, this).bind()), this.oApp.registerBrowserEvent(this.elBtnMergeCell, "click", "TE_MERGE_CELLS"), this.oApp.registerBrowserEvent(this.elBtnSplitColumn, "click", "TE_SPLIT_COLUMN"), this.oApp.registerBrowserEvent(this.elBtnSplitRow, "click", "TE_SPLIT_ROW"), this.oApp.registerBrowserEvent(this.elBtnAddColumnRight, "click", "TE_INSERT_COLUMN_RIGHT"), this.oApp.registerBrowserEvent(this.elBtnAddRowBelow, "click", "TE_INSERT_ROW_BELOW"), this.oApp.registerBrowserEvent(this.elBtnDeleteColumn, "click", "TE_DELETE_COLUMN"), this.oApp.registerBrowserEvent(this.elBtnDeleteRow, "click", "TE_DELETE_ROW"), this.oApp.registerBrowserEvent(this.elInputRadioBGColor, "click", "DRAW_QE_RADIO_OPTION", [2]), this.oApp.registerBrowserEvent(this.elInputRadioBGImg, "click", "DRAW_QE_RADIO_OPTION", [3]), this.oApp.registerBrowserEvent(this.elInputRadioTemplate, "click", "DRAW_QE_RADIO_OPTION", [4]), this.oApp.registerBrowserEvent(this.elBtnBGPalette, "click", "TABLE_QE_TOGGLE_BGC_PALETTE"), this.oApp.registerBrowserEvent(this.elBtnBGIMGPalette, "click", "TABLE_QE_TOGGLE_IMG_PALETTE"), this.oApp.registerBrowserEvent(this.elPanelBGIMGPaletteHolder, "click", "TABLE_QE_SET_IMG_FROM_PALETTE"), this.oApp.registerBrowserEvent(this.elBtnQETemplate, "click", "TABLE_QE_TOGGLE_TEMPLATE"), this.oApp.registerBrowserEvent(document.body, "mouseup", "EVENT_OUTER_DOC_MOUSEUP"), this.oApp.registerBrowserEvent(document.body, "mousemove", "EVENT_OUTER_DOC_MOUSEMOVE")
            },
            $LOCAL_BEFORE_FIRST: function(e) {
                if (-1 < e.indexOf("REGISTER_CONVERTERS")) return this.oApp.acceptLocalBeforeFirstAgain(this, !0), !0;
                this.htResizing = {}, this.nDraggableCellEdge = 2;
                var t = jindo.$Element(document.body);
                this.nPageLeftRightMargin = parseInt(t.css("marginLeft"), 10) + parseInt(t.css("marginRight"), 10), this.nPageTopBottomMargin = parseInt(t.css("marginTop"), 10) + parseInt(t.css("marginBottom"), 10), this.QE_DIM_MERGE_BTN = 1, this.QE_DIM_BG_COLOR = 2, this.QE_DIM_REVIEW_BG_IMG = 3, this.QE_DIM_TABLE_TEMPLATE = 4, this.rxLastDigits = RegExp("([0-9]+)$"), this._assignHTMLObjects(), this.addCSSClass(this.CELL_SELECTION_CLASS, "background-color:#B4C9E9;"), this._createCellResizeGrip(), this.elIFrame = this.oApp.getWYSIWYGWindow().frameElement, this.sEmptyTDSrc = "", this.oApp.oNavigator.firefox ? this.sEmptyTDSrc = "<p><br/></p>" : this.sEmptyTDSrc = "<p>&nbsp;</p>", this._changeTableEditorStatus(this.STATUS.S_0), this._attachEvents(), this._rxCellNames = new RegExp("^(" + this._aCellName.join("|") + ")$", "i")
            },
            $ON_EVENT_EDITING_AREA_KEYUP: function(e) {
                var t = e.key();
                if (229 != t.keyCode && !t.alt && !t.ctrl && 16 != t.keyCode) switch (8 != t.keyCode && 46 != t.keyCode || (this.oApp.exec("DELETE_BLOCK_CONTENTS"), e.stop()), this.nStatus) {
                    case this.STATUS.CELL_SELECTED:
                        this._changeTableEditorStatus(this.STATUS.S_0)
                }
            },
            $ON_TABLE_QE_SELECT_TEMPLATE: function(e) {
                var t = this.rxLastDigits.exec(e.element.className),
                    i = this.elSelectionStartTable;
                this._changeTableEditorStatus(this.STATUS.S_0), this.oApp.exec("STYLE_TABLE", [i, t[1]]);
                var s = i && i.parentNode ? i.parentNode : null,
                    o = i ? null : "BODY";
                this.oApp.exec("RECORD_UNDO_ACTION", ["CHANGE_TABLE_STYLE", {
                    elSaveTarget: s,
                    sSaveTarget: o,
                    bDontSaveSelection: !0
                }])
            },
            $BEFORE_CHANGE_EDITING_MODE: function(e) {
                "WYSIWYG" !== e && this.nStatus !== this.STATUS.S_0 && this._changeTableEditorStatus(this.STATUS.S_0)
            },
            $ON_TABLE_QE_TOGGLE_BGC_PALETTE: function() {
                "block" == this.elPanelBGPaletteHolder.parentNode.style.display ? this.oApp.exec("HIDE_TABLE_QE_BGC_PALETTE", []) : this.oApp.exec("SHOW_TABLE_QE_BGC_PALETTE", [])
            },
            $ON_SHOW_TABLE_QE_BGC_PALETTE: function() {
                this.elPanelBGPaletteHolder.parentNode.style.display = "block", this.oApp.exec("SHOW_COLOR_PALETTE", ["TABLE_QE_SET_BGC_FROM_PALETTE", this.elPanelBGPaletteHolder])
            },
            $ON_HIDE_TABLE_QE_BGC_PALETTE: function() {
                this.elPanelBGPaletteHolder.parentNode.style.display = "none", this.oApp.exec("HIDE_COLOR_PALETTE", [])
            },
            $ON_TABLE_QE_SET_BGC_FROM_PALETTE: function(e) {
                this.oApp.exec("TABLE_QE_SET_BGC", [e]), this.oSelection && this.oSelection.select(), this._changeTableEditorStatus(this.STATUS.S_0)
            },
            $ON_TABLE_QE_SET_BGC: function(e) {
                this.elBtnBGPalette.style.backgroundColor = e;
                for (var t = 0, i = this.aSelectedCells.length; t < i; t++) this.aSelectedCells[t].setAttribute(this.TMP_BGC_ATTR, e), this.aSelectedCells[t].removeAttribute(this.TMP_BGIMG_ATTR);
                this.sQEAction = "TABLE_SET_BGCOLOR"
            },
            $ON_TABLE_QE_TOGGLE_IMG_PALETTE: function() {
                "block" == this.elPanelBGIMGPaletteHolder.parentNode.style.display ? this.oApp.exec("HIDE_TABLE_QE_IMG_PALETTE", []) : this.oApp.exec("SHOW_TABLE_QE_IMG_PALETTE", [])
            },
            $ON_SHOW_TABLE_QE_IMG_PALETTE: function() {
                this.elPanelBGIMGPaletteHolder.parentNode.style.display = "block"
            },
            $ON_HIDE_TABLE_QE_IMG_PALETTE: function() {
                this.elPanelBGIMGPaletteHolder.parentNode.style.display = "none"
            },
            $ON_TABLE_QE_SET_IMG_FROM_PALETTE: function(e) {
                this.oApp.exec("TABLE_QE_SET_IMG", [e.element]), this.oSelection && this.oSelection.select(), this._changeTableEditorStatus(this.STATUS.S_0)
            },
            $ON_TABLE_QE_SET_IMG: function(e) {
                for (var t = jindo.$Element(e).className(), i = jindo.$Element(this.elBtnBGIMGPalette), s = i.className().split(" "), o = 0, n = s.length; o < n; o++) 0 < s[o].indexOf("cellimg") && i.removeClass(s[o]);
                jindo.$Element(this.elBtnBGIMGPalette).addClass(t);
                var l = t.substring(11, t.length),
                    r = "pattern_";
                if ("0" === l)
                    for (o = 0, n = this.aSelectedCells.length; o < n; o++) jindo.$Element(this.aSelectedCells[o]).css("backgroundImage", ""), this.aSelectedCells[o].removeAttribute(this.TMP_BGC_ATTR), this.aSelectedCells[o].removeAttribute(this.TMP_BGIMG_ATTR);
                else {
                    r = 19 == l || 20 == l || 21 == l || 22 == l || 25 == l || 26 == l ? r + l + ".jpg" : r + l + ".gif";
                    var a = nhn.husky.SE2M_Configuration.LinkageDomain.sCommonStatic + "/static/img/" + r;
                    for (o = 0, n = this.aSelectedCells.length; o < n; o++) jindo.$Element(this.aSelectedCells[o]).css("backgroundImage", "url(" + a + ")"), this.aSelectedCells[o].removeAttribute(this.TMP_BGC_ATTR), this.aSelectedCells[o].setAttribute(this.TMP_BGIMG_ATTR, "url(" + a + ")")
                }
                this.sQEAction = "TABLE_SET_BGIMAGE"
            },
            $ON_SAVE_QE_MY_REVIEW_ITEM: function() {
                this.oApp.exec("SAVE_MY_REVIEW_ITEM"), this.oApp.exec("CLOSE_QE_LAYER")
            },
            $ON_SHOW_COMMON_QE: function() {
                jindo.$Element(this.elSelectionStartTable).hasClass(this._sSETblClass) ? this.oApp.exec("SHOW_TABLE_QE") : jindo.$Element(this.elSelectionStartTable).hasClass(this._sSEReviewTblClass) && this.oApp.exec("SHOW_REVIEW_QE")
            },
            $ON_SHOW_TABLE_QE: function() {
                this.oApp.exec("HIDE_TABLE_QE_BGC_PALETTE", []), this.oApp.exec("TABLE_QE_HIDE_TEMPLATE", []), this.oApp.exec("SETUP_TABLE_QE_MODE", [0]), this.oApp.exec("OPEN_QE_LAYER", [this.htMap[this.htSelectionEPos.x][this.htSelectionEPos.y], this.elQELayer, "table"])
            },
            $ON_SHOW_REVIEW_QE: function() {
                this.oApp.exec("SETUP_TABLE_QE_MODE", [1]), this.oApp.exec("OPEN_QE_LAYER", [this.htMap[this.htSelectionEPos.x][this.htSelectionEPos.y], this.elQELayer, "review"])
            },
            $ON_CLOSE_SUB_LAYER_QE: function() {
                "undefined" != typeof this.elPanelBGPaletteHolder && (this.elPanelBGPaletteHolder.parentNode.style.display = "none"), "undefined" != typeof this.elPanelBGIMGPaletteHolder && (this.elPanelBGIMGPaletteHolder.parentNode.style.display = "none")
            },
            $ON_SETUP_TABLE_QE_MODE: function(e) {
                var t = !0;
                "number" == typeof e && (this.nQEMode = e), this.aSelectedCells.length < 2 && (t = !1), this.oApp.exec("TABLE_QE_DIM", [this.QE_DIM_MERGE_BTN, t]);
                for (var i = this.aSelectedCells[0].getAttribute(this.TMP_BGC_ATTR) || "rgb(255,255,255)", s = !0, o = 1, n = this.aSelectedCells.length; o < n; o++)
                    if (this.aSelectedCells[o] && i != this.aSelectedCells[o].getAttribute(this.TMP_BGC_ATTR)) {
                        s = !1;
                        break
                    } this.elBtnBGPalette.style.backgroundColor = s ? i : "#FFFFFF";
                var l, r = this.aSelectedCells[0].getAttribute(this.TMP_BGIMG_ATTR) || "",
                    a = !0,
                    h = 0,
                    _ = jindo.$Element(this.elBtnBGIMGPalette);
                if (r) {
                    var p = r.match(/_[0-9]*/);
                    for (h = (l = p ? p[0] : "_0").substring(1, l.length), o = 1, n = this.aSelectedCells.length; o < n; o++)
                        if (r != this.aSelectedCells[o].getAttribute(this.TMP_BGIMG_ATTR)) {
                            a = !1;
                            break
                        }
                }
                var d = _.className().split(/\s/);
                n = d.length;
                for (var c = 0; c < n; c++) 0 < d[c].indexOf("cellimg") && _.removeClass(d[c]);
                if (a && 0 < h ? _.addClass("se2_cellimg" + h) : _.addClass("se2_cellimg0"), 0 === this.nQEMode) {
                    this.elPanelTableTemplateArea.style.display = "block", this.elPanelReviewBGArea.style.display = "none", jindo.$Element(this.elPanelTableBGArea).className("se2_qe2");
                    var E = this.parseIntOr0(this.elSelectionStartTable.getAttribute(this.ATTR_TBL_TEMPLATE));
                    E || (this.elInputRadioBGColor.checked = "true", E = 1), this.elPanelQETemplatePreview.className = "se2_t_style" + E, this.elPanelBGImg.style.position = ""
                } else 1 == this.nQEMode ? (this.elPanelTableTemplateArea.style.display = "none", this.elPanelReviewBGArea.style.display = "block", E = this.parseIntOr0(this.elSelectionStartTable.getAttribute(this.ATTR_REVIEW_TEMPLATE)), this.elPanelBGImg.style.position = "relative") : (this.elPanelTableTemplateArea.style.display = "none", this.elPanelReviewBGArea.style.display = "none");
                this.oApp.exec("DRAW_QE_RADIO_OPTION", [0])
            },
            $ON_DRAW_QE_RADIO_OPTION: function(e) {
                0 !== e && 2 != e && this.oApp.exec("HIDE_TABLE_QE_BGC_PALETTE", []), 0 !== e && 3 != e && this.oApp.exec("HIDE_TABLE_QE_IMG_PALETTE", []), 0 !== e && 4 != e && this.oApp.exec("TABLE_QE_HIDE_TEMPLATE", []), 0 === this.nQEMode ? (this.elInputRadioBGImg.checked && (this.elInputRadioBGColor.checked = "true"), this.elInputRadioBGColor.checked ? this.oApp.exec("TABLE_QE_DIM", [this.QE_DIM_TABLE_TEMPLATE, !1]) : this.oApp.exec("TABLE_QE_DIM", [this.QE_DIM_BG_COLOR, !1])) : (this.elInputRadioTemplate.checked && (this.elInputRadioBGColor.checked = "true"), this.elInputRadioBGColor.checked ? this.oApp.exec("TABLE_QE_DIM", [this.QE_DIM_REVIEW_BG_IMG, !1]) : this.oApp.exec("TABLE_QE_DIM", [this.QE_DIM_BG_COLOR, !1]))
            },
            $ON_TABLE_QE_DIM: function(e, t) {
                var i;
                i = 1 == e ? this.elPanelDim1 : this.elPanelDim2, t && (e = 0), i.className = "se2_qdim" + e
            },
            $ON_TE_SELECT_TABLE: function(e) {
                this.elSelectionStartTable = e, this.htMap = this._getCellMapping(this.elSelectionStartTable)
            },
            $ON_TE_SELECT_CELLS: function(e, t) {
                this._selectCells(e, t)
            },
            $ON_TE_MERGE_CELLS: function() {
                if (0 !== this.aSelectedCells.length && 1 != this.aSelectedCells.length) {
                    var e, t, i, s, o;
                    this._removeClassFromSelection(), t = this.aSelectedCells[0];
                    var n, l = this.aSelectedCells[0];
                    for (s = parseInt(l.style.height || l.getAttribute("height"), 10), o = parseInt(l.style.width || l.getAttribute("width"), 10), e = this.htSelectionSPos.x + 1; e < this.htSelectionEPos.x + 1; e++)(n = this.htMap[e][this.htSelectionSPos.y]) != l && (l = n, o += parseInt(n.style.width || n.getAttribute("width"), 10));
                    for (l = this.aSelectedCells[0], e = this.htSelectionSPos.y + 1; e < this.htSelectionEPos.y + 1; e++)(n = this.htMap[this.htSelectionSPos.x][e]) != l && (l = n, s += parseInt(n.style.height || n.getAttribute("height"), 10));
                    for (o && (t.style.width = o + "px"), s && (t.style.height = s + "px"), t.setAttribute("colSpan", this.htSelectionEPos.x - this.htSelectionSPos.x + 1), t.setAttribute("rowSpan", this.htSelectionEPos.y - this.htSelectionSPos.y + 1), e = 1; e < this.aSelectedCells.length; e++)
                        if ((i = this.aSelectedCells[e]).parentNode) {
                            nhn.husky.SE2M_Utils.isBlankNode(i) || (t.innerHTML += i.innerHTML);
                            var r = jindo.$Agent().navigator();
                            !r.ie || 9 != r.nativeVersion && 10 != r.nativeVersion || 9 != r.version && 10 != r.version || this._removeEmptyTextNode_IE(i), i.parentNode.removeChild(i)
                        } this.htMap = this._getCellMapping(this.elSelectionStartTable), this._selectCells(this.htSelectionSPos, this.htSelectionEPos), this._showTableTemplate(this.elSelectionStartTable), this._addClassToSelection(), this.sQEAction = "TABLE_CELL_MERGE", this.oApp.exec("SHOW_COMMON_QE")
                }
            },
            $ON_TABLE_QE_TOGGLE_TEMPLATE: function() {
                "block" == this.elPanelQETemplate.style.display ? this.oApp.exec("TABLE_QE_HIDE_TEMPLATE") : this.oApp.exec("TABLE_QE_SHOW_TEMPLATE")
            },
            $ON_TABLE_QE_SHOW_TEMPLATE: function() {
                this.elPanelQETemplate.style.display = "block", this.oApp.exec("POSITION_TOOLBAR_LAYER", [this.elPanelQETemplate])
            },
            $ON_TABLE_QE_HIDE_TEMPLATE: function() {
                this.elPanelQETemplate.style.display = "none"
            },
            $ON_STYLE_TABLE: function(e, t) {
                e || (this._t || (this._t = 1), e = this.elSelectionStartTable, t = this._t++ % 20 + 1), this.oSelection && this.oSelection.select(), this._applyTableTemplate(e, t)
            },
            $ON_TE_DELETE_COLUMN: function() {
                0 !== this.aSelectedCells.length && (this._selectAll_Column(), this._deleteSelectedCells(), this.sQEAction = "DELETE_TABLE_COLUMN", this._changeTableEditorStatus(this.STATUS.S_0))
            },
            $ON_TE_DELETE_ROW: function() {
                0 !== this.aSelectedCells.length && (this._selectAll_Row(), this._deleteSelectedCells(), this.sQEAction = "DELETE_TABLE_ROW", this._changeTableEditorStatus(this.STATUS.S_0))
            },
            $ON_TE_INSERT_COLUMN_RIGHT: function() {
                0 !== this.aSelectedCells.length && (this._selectAll_Column(), this._insertColumnAfter(this.htSelectionEPos.x))
            },
            $ON_TE_INSERT_COLUMN_LEFT: function() {
                this._selectAll_Column(), this._insertColumnAfter(this.htSelectionSPos.x - 1)
            },
            $ON_TE_INSERT_ROW_BELOW: function() {
                0 !== this.aSelectedCells.length && this._insertRowBelow(this.htSelectionEPos.y)
            },
            $ON_TE_INSERT_ROW_ABOVE: function() {
                this._insertRowBelow(this.htSelectionSPos.y - 1)
            },
            $ON_TE_SPLIT_COLUMN: function() {
                var e, t, i, s, o, n;
                if (0 !== this.aSelectedCells.length) {
                    this._removeClassFromSelection();
                    for (var l = this.aSelectedCells[0], r = 0, a = this.aSelectedCells.length; r < a; r++)
                        if (o = this.aSelectedCells[r], !(1 < (e = parseInt(o.getAttribute("colSpan"), 10) || 1)))
                            for (var h = this._getBasisCellPosition(o), _ = 0; _ < this.htMap[0].length;) o = this.htMap[h.x][_], e = parseInt(o.getAttribute("colSpan"), 10) || 1, o.setAttribute("colSpan", e + 1), _ += parseInt(o.getAttribute("rowSpan"), 10) || 1;
                    for (r = 0, a = this.aSelectedCells.length; r < a; r++) {
                        o = this.aSelectedCells[r], t = ((e = parseInt(o.getAttribute("colSpan"), 10) || 1) / 2).toFixed(0), o.setAttribute("colSpan", t), (n = this._shallowCloneTD(o)).setAttribute("colSpan", e - t), l = n, e = parseInt(o.getAttribute("rowSpan"), 10) || 1, n.setAttribute("rowSpan", e), n.innerHTML = "&nbsp;", (i = o.width || o.style.width) && (i = this.parseIntOr0(i), o.removeAttribute("width"), s = (i / 2).toFixed(), o.style.width = s + "px", n.style.width = i - s + "px"), o.parentNode.insertBefore(n, o.nextSibling);
                        var p = jindo.$Agent().navigator();
                        (p.edge && 12 === Math.floor(p.version) || p.ie && (9 <= p.nativeVersion || p.nativeVersion <= 11) && (9 <= p.version || p.version <= 11)) && (n.style.cssText = o.style.cssText)
                    }
                    this._reassignCellSizes(this.elSelectionStartTable), this.htMap = this._getCellMapping(this.elSelectionStartTable), h = this._getBasisCellPosition(l), this.htSelectionEPos.x = h.x, this._selectCells(this.htSelectionSPos, this.htSelectionEPos), this.sQEAction = "SPLIT_TABLE_COLUMN", this.oApp.exec("SHOW_COMMON_QE")
                }
            },
            $ON_TE_SPLIT_ROW: function() {
                var e, t, i, s, o, n, l;
                if (0 !== this.aSelectedCells.length) {
                    var r = jindo.$$(">TBODY>TR", this.elSelectionStartTable, {
                        oneTimeOffCache: !0
                    });
                    this._removeClassFromSelection();
                    for (var a, h, _, p = 0, d = 0, c = this.aSelectedCells.length; d < c; d++)
                        if (s = this.aSelectedCells[d], !(1 < (e = parseInt(s.getAttribute("rowSpan"), 10) || 1))) {
                            a = r[(n = this._getBasisCellPosition(s)).y], l = this.oApp.getWYSIWYGDocument().createElement("TR"), a.parentNode.insertBefore(l, a.nextSibling), p++;
                            for (var E = 0; E < this.htMap.length;) s = this.htMap[E][n.y], e = parseInt(s.getAttribute("rowSpan"), 10) || 1, s.setAttribute("rowSpan", e + 1), E += parseInt(s.getAttribute("colSpan"), 10) || 1
                        } for (r = jindo.$$(">TBODY>TR", this.elSelectionStartTable, {
                            oneTimeOffCache: !0
                        }), d = 0, c = this.aSelectedCells.length; d < c; d++) {
                        if (s = this.aSelectedCells[d], t = ((e = parseInt(s.getAttribute("rowSpan"), 10) || 1) / 2).toFixed(0), s.setAttribute("rowSpan", t), (o = this._shallowCloneTD(s)).setAttribute("rowSpan", e - t), e = parseInt(s.getAttribute("colSpan"), 10) || 1, o.setAttribute("colSpan", e), o.innerHTML = "&nbsp;", i = s.height || s.style.height) {
                            i = this.parseIntOr0(i), s.removeAttribute("height");
                            var T = (i / 2).toFixed();
                            s.style.height = T + "px", o.style.height = i - T + "px"
                        }
                        var A, S = jindo.$A(r).indexOf(s.parentNode),
                            u = r[parseInt(S, 10) + parseInt(t, 10)],
                            g = u.childNodes,
                            C = null;
                        h = this._getBasisCellPosition(s);
                        for (var f = 0, L = g.length; f < L; f++)
                            if ((A = g[f]).tagName && this._rxCellNames.test(A.tagName) && (_ = this._getBasisCellPosition(A), h.x < _.x)) {
                                C = A;
                                break
                            } u.insertBefore(o, C);
                        var y = jindo.$Agent().navigator();
                        (y.edge && 12 === Math.floor(y.version) || y.ie && (9 <= y.nativeVersion || y.nativeVersion <= 11) && (9 <= y.version || y.version <= 11)) && (o.style.cssText = o.style.cssText)
                    }
                    this._reassignCellSizes(this.elSelectionStartTable), this.htMap = this._getCellMapping(this.elSelectionStartTable), this.htSelectionEPos.y += p, this._selectCells(this.htSelectionSPos, this.htSelectionEPos), this.sQEAction = "SPLIT_TABLE_ROW", this.oApp.exec("SHOW_COMMON_QE")
                }
            },
            $ON_MSG_CELL_SELECTED: function() {
                this.elPanelDimDelCol.className = "se2_qdim6r", this.elPanelDimDelRow.className = "se2_qdim6c", 0 === this.htSelectionSPos.x && this.htSelectionEPos.x === this.htMap.length - 1 && this.oApp.exec("MSG_ROW_SELECTED"), 0 === this.htSelectionSPos.y && this.htSelectionEPos.y === this.htMap[0].length - 1 && this.oApp.exec("MSG_COL_SELECTED"), this.oApp.exec("SHOW_COMMON_QE")
            },
            $ON_MSG_ROW_SELECTED: function() {
                this.elPanelDimDelRow.className = ""
            },
            $ON_MSG_COL_SELECTED: function() {
                this.elPanelDimDelCol.className = ""
            },
            $ON_EVENT_EDITING_AREA_MOUSEDOWN: function(e) {
                if (this.oApp.isWYSIWYGEnabled()) switch (this.nStatus) {
                    case this.STATUS.S_0:
                        if (!e.element) return;
                        if ("IMG" == e.element.tagName) return;
                        if ("WYSIWYG" !== this.oApp.getEditingMode()) return;
                        var t = nhn.husky.SE2M_Utils.findClosestAncestorAmongTagNames(this._aCellName, e.element);
                        if (t && this._rxCellNames.test(t.tagName)) {
                            var i = nhn.husky.SE2M_Utils.findAncestorByTagName("TABLE", t);
                            if (!jindo.$Element(i).hasClass(this._sSETblClass) && !jindo.$Element(i).hasClass(this._sSEReviewTblClass)) return;
                            if (!this._isValidTable(i)) return jindo.$Element(i).removeClass(this._sSETblClass), void jindo.$Element(i).removeClass(this._sSEReviewTblClass);
                            i && (this.elSelectionStartTD = t, this.elSelectionStartTable = i, this._changeTableEditorStatus(this.STATUS.MOUSEDOWN_CELL))
                        }
                        break;
                    case this.STATUS.MOUSEDOWN_CELL:
                    case this.STATUS.CELL_SELECTING:
                        break;
                    case this.STATUS.CELL_SELECTED:
                        this._changeTableEditorStatus(this.STATUS.S_0)
                }
            },
            $ON_EVENT_EDITING_AREA_MOUSEMOVE: function(e) {
                if ("WYSIWYG" == this.oApp.getEditingMode()) {
                    var t = e.element,
                        i = jindo.$Agent().navigator(),
                        s = "onpointerup",
                        o = "onresizeend";
                    switch (i.ie && t && t.tagName && "TABLE" === t.tagName.toUpperCase() && (s in t ? t[s] = this._wfnOnResizeEndTable : o in t && (8 < i.version ? t[o] = this._wfnOnResizeEndTable : t[o] = this._getTableResizeEndHandler(t))), this.nStatus) {
                        case this.STATUS.S_0:
                            this._isOnBorder(e) ? this._showCellResizeGrip(e) : this._hideResizer();
                            break;
                        case this.STATUS.MOUSEDOWN_CELL:
                            var n = nhn.husky.SE2M_Utils.findClosestAncestorAmongTagNames(this._aCellName, e.element);
                            (n && n !== this.elSelectionStartTD || !n) && (n = n || this.elSelectionStartTD, this._reassignCellSizes(this.elSelectionStartTable), this._startCellSelection(), this._selectBetweenCells(this.elSelectionStartTD, n));
                            break;
                        case this.STATUS.CELL_SELECTING:
                            if (!(n = nhn.husky.SE2M_Utils.findClosestAncestorAmongTagNames(this._aCellName, e.element)) || n === this.elLastSelectedTD) return;
                            if (nhn.husky.SE2M_Utils.findAncestorByTagName("TABLE", n) !== this.elSelectionStartTable) return;
                            this.elLastSelectedTD = n, this._selectBetweenCells(this.elSelectionStartTD, n);
                            break;
                        case this.STATUS.CELL_SELECTED:
                    }
                }
            },
            $ON_EVENT_OUTER_DOC_MOUSEMOVE: function(e) {
                switch (this.nStatus) {
                    case this.STATUS.CELL_SELECTING:
                        var t = e.pos(),
                            i = t.pageY,
                            s = t.pageX;
                        if (i < this.htEditingAreaPos.top) {
                            var o = this.htSelectionSPos.y;
                            if (0 < o) {
                                this.htSelectionSPos.y--, this._selectCells(this.htSelectionSPos, this.htSelectionEPos);
                                var n = this.oApp.getSelection();
                                n.selectNodeContents(this.aSelectedCells[0]), n.select(), n.oBrowserSelection.selectNone()
                            }
                        } else i > this.htEditingAreaPos.bottom && (o = this.htSelectionEPos.y) < this.htMap[0].length - 1 && (this.htSelectionEPos.y++, this._selectCells(this.htSelectionSPos, this.htSelectionEPos), (n = this.oApp.getSelection()).selectNodeContents(this.htMap[this.htSelectionEPos.x][this.htSelectionEPos.y]), n.select(), n.oBrowserSelection.selectNone());
                        if (s < this.htEditingAreaPos.left) {
                            var l = this.htSelectionSPos.x;
                            0 < l && (this.htSelectionSPos.x--, this._selectCells(this.htSelectionSPos, this.htSelectionEPos), (n = this.oApp.getSelection()).selectNodeContents(this.aSelectedCells[0]), n.select(), n.oBrowserSelection.selectNone())
                        } else s > this.htEditingAreaPos.right && (l = this.htSelectionEPos.x) < this.htMap.length - 1 && (this.htSelectionEPos.x++, this._selectCells(this.htSelectionSPos, this.htSelectionEPos), (n = this.oApp.getSelection()).selectNodeContents(this.htMap[this.htSelectionEPos.x][this.htSelectionEPos.y]), n.select(), n.oBrowserSelection.selectNone())
                }
            },
            $ON_EVENT_OUTER_DOC_MOUSEUP: function(e) {
                this._eventEditingAreaMouseup(e)
            },
            $ON_EVENT_EDITING_AREA_MOUSEUP: function(e) {
                this._eventEditingAreaMouseup(e)
            },
            _eventEditingAreaMouseup: function() {
                if ("WYSIWYG" == this.oApp.getEditingMode()) switch (this.nStatus) {
                    case this.STATUS.S_0:
                        break;
                    case this.STATUS.MOUSEDOWN_CELL:
                        this._changeTableEditorStatus(this.STATUS.S_0);
                        break;
                    case this.STATUS.CELL_SELECTING:
                        this._changeTableEditorStatus(this.STATUS.CELL_SELECTED);
                        break;
                    case this.STATUS.CELL_SELECTED:
                }
            },
            $ON_GET_SELECTED_CELLS: function(e, t) {
                this.aSelectedCells && (t[e] = this.aSelectedCells)
            },
            _coverResizeLayer: function() {
                this.elResizeCover.style.position = "fixed";
                var e = jindo.$Document().clientSize();
                this.elResizeCover.style.width = e.width - this.nPageLeftRightMargin + "px", this.elResizeCover.style.height = e.height - this.nPageTopBottomMargin + "px", document.body.appendChild(this.elResizeCover)
            },
            _uncoverResizeLayer: function() {
                this.elResizeGrid.appendChild(this.elResizeCover), this.elResizeCover.style.position = "", this.elResizeCover.style.width = "100%", this.elResizeCover.style.height = "100%"
            },
            _reassignCellSizes: function(e) {
                var t = new Array(2);
                t[0] = jindo.$$(">TBODY>TR>TD", e, {
                    oneTimeOffCache: !0
                }), t[1] = jindo.$$(">TBODY>TR>TH", e, {
                    oneTimeOffCache: !0
                });
                for (var i = new Array(t[0].length + t[1].length), s = 0, o = 0; o < 2; o++)
                    for (var n = 0; n < t[o].length; n++) {
                        var l, r, a, h, _, p, d, c, E, T, A = t[o][n],
                            S = jindo.$Element(A);
                        c = A.getComputedStyle ? (l = parseFloat(getComputedStyle(A).paddingLeft, 10), r = parseFloat(getComputedStyle(A).paddingRight, 10), a = parseFloat(getComputedStyle(A).paddingTop, 10), h = parseFloat(getComputedStyle(A).paddingBottom, 10), _ = parseFloat(getComputedStyle(A).borderLeftWidth, 10), p = parseFloat(getComputedStyle(A).borderRightWidth, 10), d = parseFloat(getComputedStyle(A).borderTopWidth, 10), parseFloat(getComputedStyle(A).borderBottomWidth, 10)) : (l = this.parseIntOr0(A.style.paddingLeft), r = this.parseIntOr0(A.style.paddingRight), a = this.parseIntOr0(A.style.paddingTop), h = this.parseIntOr0(A.style.paddingBottom), _ = this.parseBorder(S.css("borderLeftWidth"), S.css("borderLeftStyle")), p = this.parseBorder(S.css("borderRightWidth"), S.css("borderRightStyle")), d = this.parseBorder(S.css("borderTopWidth"), S.css("borderTopStyle")), this.parseBorder(S.css("borderBottomWidth"), S.css("borderBottomStyle")));
                        var u = jindo.$Element(A).attr("width"),
                            g = jindo.$Element(A).attr("height");
                        T = u || g ? (E = A.offsetWidth - (l + r + _ + p) + "px", A.offsetHeight - (a + h + d + c) + "px") : (E = A.style.width, A.style.height), i[s++] = [A, E, T]
                    }
                var C = e._nWidth,
                    f = e._nHeight,
                    L = e._nResizedWidth,
                    y = e._nResizedHeight,
                    O = 1,
                    B = 1;
                for (nhn.husky.SE2M_Utils.isNumber(C) && nhn.husky.SE2M_Utils.isNumber(f) && nhn.husky.SE2M_Utils.isNumber(L) && nhn.husky.SE2M_Utils.isNumber(y) && (O = L / C, B = y / f), e._nResizedWidth && nhn.husky.SE2M_Utils.deleteProperty(e, "_nResizedWidth"), e._nResizedHeight && nhn.husky.SE2M_Utils.deleteProperty(e, "_nResizedHeight"), n = 0; n < s; n++) {
                    var R = i[n];
                    R[0].removeAttribute("width"), R[0].removeAttribute("height"), R[0].style.width = parseFloat(R[1], 10) * O + "px", R[0].style.height = parseFloat(R[2], 10) * B + "px"
                }
                var b = jindo.$Element(e);
                e._nWidth = b.width(), e._nHeight = b.height(), e.removeAttribute("width"), e.removeAttribute("height"), e.style.width = "", e.style.height = ""
            },
            _fnOnMouseDownResizeCover: function(e) {
                this.bResizing = !0, this.nStartHeight = e.pos().clientY, this.bResizingCover = !0, this._wfnOnMouseMoveResizeCover.attach(this.elResizeCover, "mousemove"), this._wfnOnMouseUpResizeCover.attach(document, "mouseup"), this._coverResizeLayer(), this.elResizeGrid.style.border = "1px dotted black", this.nStartHeight = e.pos().clientY, this.nStartWidth = e.pos().clientX, this.nClientXDiff = this.nStartWidth - this.htResizing.htEPos.clientX, this.nClientYDiff = this.nStartHeight - this.htResizing.htEPos.clientY, this._reassignCellSizes(this.htResizing.elTable), this.htMap = this._getCellMapping(this.htResizing.elTable);
                var t = this._getBasisCellPosition(this.htResizing.elCell),
                    i = (parseInt(this.htResizing.elCell.getAttribute("colspan")) || 1) - 1,
                    s = (parseInt(this.htResizing.elCell.getAttribute("rowspan")) || 1) - 1,
                    o = t.x + i + this.htResizing.nHA,
                    n = t.y + s + this.htResizing.nVA;
                o < 0 || n < 0 || (this.htAllAffectedCells = this._getAllAffectedCells(o, n, this.htResizing.nResizeMode, this.htResizing.elTable))
            },
            _fnOnMouseMoveResizeCover: function(e) {
                (jindo.$Agent().navigator().chrome || jindo.$Agent().navigator().safari) && this.htResizing.nPreviousResizeMode != undefined && 0 != this.htResizing.nPreviousResizeMode && this.htResizing.nResizeMode != this.htResizing.nPreviousResizeMode && (this.htResizing.nResizeMode = this.htResizing.nPreviousResizeMode, this._showResizer()), 1 == this.htResizing.nResizeMode ? this.elResizeGrid.style.left = e.pos().clientX - this.nClientXDiff - this.parseIntOr0(this.elResizeGrid.style.width) / 2 + "px" : this.elResizeGrid.style.top = e.pos().clientY - this.nClientYDiff - this.parseIntOr0(this.elResizeGrid.style.height) / 2 + "px"
            },
            _fnOnMouseUpResizeCover: function(e) {
                this.bResizing = !1, this._hideResizer(), this._uncoverResizeLayer(), this.elResizeGrid.style.border = "", this._wfnOnMouseMoveResizeCover.detach(this.elResizeCover, "mousemove"), this._wfnOnMouseUpResizeCover.detach(document, "mouseup");
                var t, i, s = 0,
                    o = 0;
                2 == this.htResizing.nResizeMode && (s = e.pos().clientY - this.nStartHeight), 1 == this.htResizing.nResizeMode && (o = e.pos().clientX - this.nStartWidth, -1 != this.htAllAffectedCells.nMinBefore && o < -1 * this.htAllAffectedCells.nMinBefore && (o = -1 * this.htAllAffectedCells.nMinBefore + this.MIN_CELL_WIDTH), -1 != this.htAllAffectedCells.nMinAfter && o > this.htAllAffectedCells.nMinAfter && (o = this.htAllAffectedCells.nMinAfter - this.MIN_CELL_WIDTH));
                for (var n = this.htAllAffectedCells.aCellsBefore, l = 0; l < n.length; l++) {
                    var r = n[l];
                    i = t = 0, t = r.style.width, t = isNaN(parseFloat(t, 10)) ? 0 : parseFloat(t, 10), t += o, i = r.style.height, i = isNaN(parseFloat(i, 10)) ? 0 : parseFloat(i, 10), i += s, r.style.width = Math.max(t, this.MIN_CELL_WIDTH) + "px", r.style.height = Math.max(i, this.MIN_CELL_HEIGHT) + "px"
                }
                var a = this.htAllAffectedCells.aCellsAfter;
                for (l = 0; l < a.length; l++) i = t = 0, t = (r = a[l]).style.width, t = isNaN(parseFloat(t, 10)) ? 0 : parseFloat(t, 10), t -= o, i = r.style.height, i = isNaN(parseFloat(i, 10)) ? 0 : parseFloat(i, 10), i -= s, r.style.width = Math.max(t, this.MIN_CELL_WIDTH) + "px", r.style.height = Math.max(i, this.MIN_CELL_HEIGHT) + "px";
                this.bResizingCover = !1
            },
            _fnOnResizeEndTable: function(e) {
                var t = jindo.$Event(e).element;
                this._markResizedMetric(t)
            },
            _getTableResizeEndHandler: function(e) {
                return jindo.$Fn(this._markResizedMetric, this).bind(e)
            },
            _markResizedMetric: function(e) {
                if (e && "TABLE" === e.tagName.toUpperCase()) {
                    var t = jindo.$Element(e);
                    e._nResizedWidth = t.width(), e._nResizedHeight = t.height()
                }
            },
            $ON_CLOSE_QE_LAYER: function(e) {
                var t, i, s = e ? e.element : null,
                    o = s ? jindo.$Element(s) : null,
                    n = ["q_open_table_fold", "q_open_table_full"],
                    l = !1;
                if (o)
                    for (i = n.length; i--;)
                        if (t = n[i], o.hasClass(t)) {
                            l = !0;
                            break
                        } l || this._changeTableEditorStatus(this.STATUS.S_0)
            },
            _changeTableEditorStatus: function(e) {
                if (this.nStatus != e) {
                    switch (this.nStatus = e) {
                        case this.STATUS.S_0:
                            if (this.nStatus == this.STATUS.MOUSEDOWN_CELL) break;
                            this._deselectCells(), this.sQEAction && (this.oApp.exec("RECORD_UNDO_ACTION", [this.sQEAction, {
                                elSaveTarget: this.elSelectionStartTable,
                                bDontSaveSelection: !0
                            }]), this.sQEAction = ""), (this.oApp.oNavigator.safari || this.oApp.oNavigator.chrome) && (this.oApp.getWYSIWYGDocument().onselectstart = null), this.oApp.exec("ENABLE_WYSIWYG", []), this.oApp.exec("CLOSE_QE_LAYER"), this.elSelectionStartTable = null;
                            break;
                        case this.STATUS.CELL_SELECTING:
                            this.oApp.oNavigator.ie && "function" == typeof document.body.setCapture && document.body.setCapture(!1);
                            break;
                        case this.STATUS.CELL_SELECTED:
                            this.oApp.delayedExec("MSG_CELL_SELECTED", [], 0), this.oApp.oNavigator.ie && "function" == typeof document.body.releaseCapture && document.body.releaseCapture()
                    }
                    this.oApp.exec("TABLE_EDITOR_STATUS_CHANGED", [this.nStatus])
                }
            },
            _isOnBorder: function(e) {
                if (this.htResizing.nResizeMode = 0, this.htResizing.elCell = e.element, !this._rxCellNames.test(e.element.tagName)) return !1;
                if (this.htResizing.elTable = nhn.husky.SE2M_Utils.findAncestorByTagName("TABLE", this.htResizing.elCell), this.htResizing.elTable && (jindo.$Element(this.htResizing.elTable).hasClass(this._sSETblClass) || jindo.$Element(this.htResizing.elTable).hasClass(this._sSEReviewTblClass))) {
                    var t, i;
                    this.htResizing.nVA = 0, this.htResizing.nHA = 0, this.htResizing.nBorderLeftPos = 0, this.htResizing.nBorderTopPos = -1, this.htResizing.htEPos = e.pos(!0), this.htResizing.nBorderSize = this.parseIntOr0(this.htResizing.elTable.border), i = jindo.$Agent().navigator().ie || jindo.$Agent().navigator().safari ? (t = this.htResizing.nBorderSize + this.nDraggableCellEdge, this.nDraggableCellEdge) : (t = this.nDraggableCellEdge, this.htResizing.nBorderSize + this.nDraggableCellEdge);
                    var s = this.htResizing.elCell.clientWidth,
                        o = this.htResizing.elCell.clientHeight,
                        n = s - this.htResizing.htEPos.offsetX,
                        l = o - this.htResizing.htEPos.offsetY;
                    if (this.htResizing.htEPos.offsetY <= t) {
                        var r = !1,
                            a = this.htResizing.elCell.parentNode,
                            h = a.parentNode;
                        jindo.$$("tr", h, {
                            oneTimeOffCache: !0
                        })[0] == a && (r = !0), r && (this.htResizing.nVA = -1, this.htResizing.nResizeMode = 4)
                    }
                    return l <= i && (this.htResizing.nBorderTopPos = this.htResizing.elCell.offsetHeight + t - 1, this.htResizing.nResizeMode = 2), this.htResizing.htEPos.offsetX <= t && this.htResizing.elTable.scrollLeft != this.htResizing.elCell.offsetLeft && (this.htResizing.nHA = -1, this.htResizing.nResizeMode = 3), n <= t && (this.htResizing.nBorderLeftPos = this.htResizing.elCell.offsetWidth + t - 1, this.htResizing.nResizeMode = 1), (jindo.$Agent().navigator().chrome || jindo.$Agent().navigator().safari) && (this.htResizing.elPreviousCell ? this.htResizing.elCell != this.htResizing.elPreviousCell && (this.htResizing.elPreviousCell = this.htResizing.elCell) : this.htResizing.elPreviousCell = this.htResizing.elCell), 0 !== this.htResizing.nResizeMode
                }
            },
            _showCellResizeGrip: function() {
                if (4 != this.htResizing.nResizeMode)
                    if (1 == this.htResizing.nResizeMode || 3 == this.htResizing.nResizeMode ? this.elResizeCover.style.cursor = "col-resize" : 2 == this.htResizing.nResizeMode && (this.elResizeCover.style.cursor = "row-resize"), this._showResizer(), 1 == this.htResizing.nResizeMode) this._setResizerSize(2 * (this.htResizing.nBorderSize + this.nDraggableCellEdge), this.parseIntOr0(jindo.$Element(this.elIFrame).css("height"))), this.elResizeGrid.style.top = "0px", this.elResizeGrid.style.left = this.htResizing.elCell.clientWidth + this.htResizing.htEPos.clientX - this.htResizing.htEPos.offsetX - this.parseIntOr0(this.elResizeGrid.style.width) / 2 + "px";
                    else if (2 == this.htResizing.nResizeMode) {
                    var e = this.oApp.elEditingAreaContainer.offsetWidth + "px";
                    this._setResizerSize(this.parseIntOr0(e), 2 * (this.htResizing.nBorderSize + this.nDraggableCellEdge)), this.elResizeGrid.style.top = this.htResizing.elCell.clientHeight + this.htResizing.htEPos.clientY - this.htResizing.htEPos.offsetY - this.parseIntOr0(this.elResizeGrid.style.height) / 2 + "px", this.elResizeGrid.style.left = "0px"
                } else 3 == this.htResizing.nResizeMode && (this._setResizerSize(2 * (this.htResizing.nBorderSize + this.nDraggableCellEdge), this.parseIntOr0(jindo.$Element(this.elIFrame).css("height"))), this.elResizeGrid.style.top = "0px", this.elResizeGrid.style.left = +this.htResizing.htEPos.clientX - this.htResizing.htEPos.offsetX - this.parseIntOr0(this.elResizeGrid.style.width) / 2 + "px", this.htResizing.nResizeMode = 1)
            },
            _getAllAffectedCells: function(e, t, i, s) {
                if (!s) return [];
                var o, n = this._getCellMapping(s),
                    l = n.length,
                    r = n[0].length,
                    a = [],
                    h = [],
                    _ = -1,
                    p = -1;
                if (1 == i) {
                    for (var d = 0; d < r; d++)
                        if (!(0 < a.length && a[a.length - 1] == n[e][d])) {
                            a[a.length] = n[e][d];
                            var c = parseInt(n[e][d].style.width);
                            (-1 == _ || c < _) && (_ = c)
                        } if (n.length > e + 1)
                        for (d = 0; d < r; d++) 0 < h.length && h[h.length - 1] == n[e + 1][d] || (h[h.length] = n[e + 1][d], c = parseInt(n[e + 1][d].style.width), (-1 == p || c < p) && (p = c));
                    o = {
                        aCellsBefore: a,
                        aCellsAfter: h,
                        nMinBefore: _,
                        nMinAfter: p
                    }
                } else {
                    for (var E = 0; E < l; E++) 0 < a.length && a[a.length - 1] == n[E][t] || (a[a.length] = n[E][t], (-1 == _ || _ > n[E][t].style.height) && (_ = n[E][t].style.height));
                    o = {
                        aCellsBefore: a,
                        aCellsAfter: h,
                        nMinBefore: _,
                        nMinAfter: p
                    }
                }
                return o
            },
            _createCellResizeGrip: function() {
                this.elTmp = document.createElement("DIV");
                try {
                    this.elTmp.innerHTML = '<div style="position:absolute; overflow:hidden; z-index: 99; "><div onmousedown="return false" style="background-color:#000000;filter:alpha(opacity=0);opacity:0.0;-moz-opacity:0.0;-khtml-opacity:0.0;cursor: col-resize; left: 0px; top: 0px; width: 100%; height: 100%;font-size:1px;z-index: 999; "></div></div>', this.elResizeGrid = this.elTmp.firstChild, this.elResizeCover = this.elResizeGrid.firstChild
                } catch (e) {}
                jindo.$$.getSingle(".husky_seditor_editing_area_container").appendChild(this.elResizeGrid)
            },
            _selectAll_Row: function() {
                this.htSelectionSPos.x = 0, this.htSelectionEPos.x = this.htMap.length - 1, this._selectCells(this.htSelectionSPos, this.htSelectionEPos)
            },
            _selectAll_Column: function() {
                this.htSelectionSPos.y = 0, this.htSelectionEPos.y = this.htMap[0].length - 1, this._selectCells(this.htSelectionSPos, this.htSelectionEPos)
            },
            _deleteSelectedCells: function() {
                for (var e, t = 0, i = this.aSelectedCells.length; t < i; t++) {
                    e = this.aSelectedCells[t];
                    var s = jindo.$Agent().navigator();
                    !s.ie || 9 != s.nativeVersion && 10 != s.nativeVersion || 9 != s.version && 10 != s.version || this._removeEmptyTextNode_IE(e), e.parentNode.removeChild(e)
                }
                var o = jindo.$$(">TBODY>TR", this.elSelectionStartTable, {
                    oneTimeOffCache: !0
                });
                if (this.htSelectionEPos.x - this.htSelectionSPos.x + 1 == this.htMap.length) {
                    for (t = 0, i = o.length; t < i; t++) e = o[t], this.htMap[0][t] && this.htMap[0][t].parentNode && "TR" === this.htMap[0][t].parentNode.tagName || (!(s = jindo.$Agent().navigator()).ie || 9 != s.nativeVersion && 10 != s.nativeVersion || 9 != s.version && 10 != s.version || this._removeEmptyTextNode_IE(e), e.parentNode.removeChild(e));
                    o = jindo.$$(">TBODY>TR", this.elSelectionStartTable, {
                        oneTimeOffCache: !0
                    })
                }
                o.length < 1 && this.elSelectionStartTable.parentNode.removeChild(this.elSelectionStartTable), this._updateSelection()
            },
            _insertColumnAfter: function() {
                this._removeClassFromSelection(), this._hideTableTemplate(this.elSelectionStartTable);
                for (var e, t, i, s, o = jindo.$$(">TBODY>TR", this.elSelectionStartTable, {
                        oneTimeOffCache: !0
                    }), n = jindo.$Agent().navigator(), l = 0, r = this.htMap[0].length; l < r; l++) {
                    i = o[l];
                    for (var a = this.htSelectionEPos.x; a >= this.htSelectionSPos.x; a--) {
                        e = this.htMap[a][l], t = this._shallowCloneTD(e);
                        var h = parseInt(e.getAttribute("rowSpan"));
                        1 < h && (t.setAttribute("rowSpan", 1), t.style.height = ""), 1 < (h = parseInt(e.getAttribute("colSpan"))) && (t.setAttribute("colSpan", 1), t.style.width = ""), s = null;
                        for (var _ = this.htSelectionEPos.x; _ >= this.htSelectionSPos.x; _--)
                            if (this.htMap[_][l].parentNode == i) {
                                s = this.htMap[_][l].nextSibling;
                                break
                            } i.insertBefore(t, s), (n.edge && 12 === Math.floor(n.version) || n.ie && (9 <= n.nativeVersion || n.nativeVersion <= 11) && (9 <= n.version || n.version <= 11)) && (t.style.cssText = t.style.cssText)
                    }
                }
                for (var p = 0, d = this.aSelectedCells.length; p < d; p++) this.aSelectedCells[p].removeAttribute("_tmp_inserted");
                var c = this.htSelectionEPos.x - this.htSelectionSPos.x + 1;
                this.htSelectionSPos.x += c, this.htSelectionEPos.x += c, this.htMap = this._getCellMapping(this.elSelectionStartTable), this._selectCells(this.htSelectionSPos, this.htSelectionEPos), this._showTableTemplate(this.elSelectionStartTable), this._addClassToSelection(), this.sQEAction = "INSERT_TABLE_COLUMN", this.oApp.exec("SHOW_COMMON_QE")
            },
            _insertRowBelow: function() {
                var e;
                this._selectAll_Row(), this._removeClassFromSelection(), this._hideTableTemplate(this.elSelectionStartTable);
                for (var t = this.htMap[0][0].parentNode.parentNode, i = jindo.$$(">TR", t, {
                        oneTimeOffCache: !0
                    })[this.htSelectionEPos.y + 1] || null, s = jindo.$Agent().navigator(), o = this.htSelectionSPos.y; o <= this.htSelectionEPos.y; o++)
                    if (e = this._getTRCloneWithAllTD(o), t.insertBefore(e, i), s.edge && 12 === Math.floor(s.version) || s.ie && (9 <= s.nativeVersion || s.nativeVersion <= 11) && (9 <= s.version || s.version <= 11)) {
                        for (var n = this.htMap[0][o].parentNode.childNodes, l = [], r = 0, a = n.length; r < a; r++) this._rxCellNames.test(n[r].nodeName) && l.push(n[r].cloneNode());
                        var h = e.childNodes;
                        for (r = 0, a = h.length; r < a; r++) {
                            var _ = h[r],
                                p = l[r];
                            this._rxCellNames.test(_.nodeName) && p && this._rxCellNames.test(p.nodeName) && (_.style.cssText = p.style.cssText)
                        }
                    } var d = this.htSelectionEPos.y - this.htSelectionSPos.y + 1;
                this.htSelectionSPos.y += d, this.htSelectionEPos.y += d, this.htMap = this._getCellMapping(this.elSelectionStartTable), this._selectCells(this.htSelectionSPos, this.htSelectionEPos), this._showTableTemplate(this.elSelectionStartTable), this._addClassToSelection(), this.sQEAction = "INSERT_TABLE_ROW", this.oApp.exec("SHOW_COMMON_QE")
            },
            _updateSelection: function() {
                this.aSelectedCells = jindo.$A(this.aSelectedCells).filter(function(e) {
                    return null !== e.parentNode && null !== e.parentNode.parentNode
                }).$value()
            },
            _startCellSelection: function() {
                this.htMap = this._getCellMapping(this.elSelectionStartTable), this.oApp.getEmptySelection().oBrowserSelection.selectNone(), (this.oApp.oNavigator.safari || this.oApp.oNavigator.chrome) && (this.oApp.getWYSIWYGDocument().onselectstart = function() {
                    return !1
                });
                var e = this.oApp.getWYSIWYGWindow().frameElement;
                this.htEditingAreaPos = jindo.$Element(e).offset(), this.htEditingAreaPos.height = e.offsetHeight, this.htEditingAreaPos.bottom = this.htEditingAreaPos.top + this.htEditingAreaPos.height, this.htEditingAreaPos.width = e.offsetWidth, this.htEditingAreaPos.right = this.htEditingAreaPos.left + this.htEditingAreaPos.width, this._changeTableEditorStatus(this.STATUS.CELL_SELECTING)
            },
            _selectBetweenCells: function(e, t) {
                this._deselectCells();
                var i = this._getBasisCellPosition(e),
                    s = this._getBasisCellPosition(t);
                this._setEndPos(i), this._setEndPos(s);
                var o = {},
                    n = {};
                o.x = Math.min(i.x, i.ex, s.x, s.ex), o.y = Math.min(i.y, i.ey, s.y, s.ey), n.x = Math.max(i.x, i.ex, s.x, s.ex), n.y = Math.max(i.y, i.ey, s.y, s.ey), this._selectCells(o, n)
            },
            _getNextCell: function(e) {
                for (; e;)
                    if ((e = e.nextSibling) && e.tagName && e.tagName.match(/^TD|TH$/)) return e;
                return null
            },
            _getCellMapping: function(e) {
                for (var t = jindo.$$(">TBODY>TR", e, {
                        oneTimeOffCache: !0
                    }), i = 0, s = t[0].childNodes, o = 0; o < s.length; o++) {
                    var n = s[o];
                    n.tagName && n.tagName.match(/^TD|TH$/) && (n.getAttribute("colSpan") ? i += this.parseIntOr0(n.getAttribute("colSpan")) : i++)
                }
                for (var l = i, r = t.length, a = new Array(l), h = 0; h < l; h++) a[h] = new Array(r);
                for (var _ = 0; _ < r; _++) {
                    var p = t[_].childNodes[0];
                    if (p)
                        for (p.tagName && p.tagName.match(/^TD|TH$/) || (p = this._getNextCell(p)), h = -1; p;)
                            if (a[++h] || (a[h] = []), !a[h][_]) {
                                for (var d = parseInt(p.getAttribute("colSpan"), 10) || 1, c = parseInt(p.getAttribute("rowSpan"), 10) || 1, E = 0; E < c; E++)
                                    for (var T = 0; T < d; T++) a[h + T] || (a[h + T] = []), a[h + T][_ + E] = p;
                                p = this._getNextCell(p)
                            }
                }
                var A, S = !1,
                    u = null,
                    g = 0,
                    C = a[0].length;
                for (_ = 0; _ < C; _++, g++)
                    if (u = null, !t[_].innerHTML.match(/TD|TH/i)) {
                        for (h = 0, A = a.length; h < A; h++)(p = a[h][_]) && p !== u && (u = p, 1 < (c = parseInt(p.getAttribute("rowSpan"), 10) || 1) && p.setAttribute("rowSpan", c - 1));
                        var f = jindo.$Agent().navigator();
                        !f.ie || 9 != f.nativeVersion && 10 != f.nativeVersion || 9 != f.version && 10 != f.version || this._removeEmptyTextNode_IE(t[_]), t[_].parentNode.removeChild(t[_]), this.htSelectionEPos.y >= g && (g--, this.htSelectionEPos.y--), S = !0
                    } return S ? this._getCellMapping(e) : a
            },
            _selectCells: function(e, t) {
                this.aSelectedCells = this._getSelectedCells(e, t), this._addClassToSelection()
            },
            _deselectCells: function() {
                this._removeClassFromSelection(), this.aSelectedCells = [], this.htSelectionSPos = {
                    x: -1,
                    y: -1
                }, this.htSelectionEPos = {
                    x: -1,
                    y: -1
                }
            },
            _addClassToSelection: function() {
                for (var e, t, i = 0; i < this.aSelectedCells.length; i++)(t = this.aSelectedCells[i]) && (null == t.ondragstart && (t.ondragstart = function() {
                    return !1
                }), (e = jindo.$Element(t)).addClass(this.CELL_SELECTION_CLASS), e.addClass("undraggable"), t.style.backgroundColor && (t.setAttribute(this.TMP_BGC_ATTR, t.style.backgroundColor), e.css("backgroundColor", "")), t.style.backgroundImage && (t.setAttribute(this.TMP_BGIMG_ATTR, t.style.backgroundImage), e.css("backgroundImage", "")))
            },
            _removeClassFromSelection: function() {
                for (var e, t, i = 0; i < this.aSelectedCells.length; i++)(t = this.aSelectedCells[i]) && ((e = jindo.$Element(t)).removeClass(this.CELL_SELECTION_CLASS), e.removeClass("undraggable"), t.getAttribute(this.TMP_BGC_ATTR) && (t.style.backgroundColor = t.getAttribute(this.TMP_BGC_ATTR), t.removeAttribute(this.TMP_BGC_ATTR)), t.getAttribute(this.TMP_BGIMG_ATTR) && (e.css("backgroundImage", t.getAttribute(this.TMP_BGIMG_ATTR)), t.removeAttribute(this.TMP_BGIMG_ATTR)))
            },
            _expandAndSelect: function(e, t) {
                var i, s, o, n;
                if (0 < e.y)
                    for (i = e.x; i <= t.x; i++)
                        if (o = this.htMap[i][e.y], this.htMap[i][e.y - 1] == o) {
                            for (n = e.y - 2; 0 <= n && this.htMap[i][n] == o;) n--;
                            return e.y = n + 1, void this._expandAndSelect(e, t)
                        } if (0 < e.x)
                    for (s = e.y; s <= t.y; s++)
                        if (o = this.htMap[e.x][s], this.htMap[e.x - 1][s] == o) {
                            for (n = e.x - 2; 0 <= n && this.htMap[n][s] == o;) n--;
                            return e.x = n + 1, void this._expandAndSelect(e, t)
                        } if (t.y < this.htMap[0].length - 1)
                    for (i = e.x; i <= t.x; i++)
                        if (o = this.htMap[i][t.y], this.htMap[i][t.y + 1] == o) {
                            for (n = t.y + 2; n < this.htMap[0].length && this.htMap[i][n] == o;) n++;
                            return t.y = n - 1, void this._expandAndSelect(e, t)
                        } if (t.x < this.htMap.length - 1)
                    for (s = e.y; s <= t.y; s++)
                        if (o = this.htMap[t.x][s], this.htMap[t.x + 1][s] == o) {
                            for (n = t.x + 2; n < this.htMap.length && this.htMap[n][s] == o;) n++;
                            return t.x = n - 1, void this._expandAndSelect(e, t)
                        }
            },
            _getSelectedCells: function(e, t) {
                this._expandAndSelect(e, t);
                var i = e.x,
                    s = e.y,
                    o = t.x,
                    n = t.y;
                this.htSelectionSPos = e, this.htSelectionEPos = t;
                for (var l = [], r = s; r <= n; r++)
                    for (var a = i; a <= o; a++) jindo.$A(l).has(this.htMap[a][r]) || (l[l.length] = this.htMap[a][r]);
                return l
            },
            _setEndPos: function(e) {
                var t, i;
                t = parseInt(e.elCell.getAttribute("colSpan"), 10) || 1, i = parseInt(e.elCell.getAttribute("rowSpan"), 10) || 1, e.ex = e.x + t - 1, e.ey = e.y + i - 1
            },
            _getBasisCellPosition: function(e) {
                var t = 0,
                    i = 0;
                for (t = 0; t < this.htMap.length; t++)
                    for (i = 0; i < this.htMap[t].length; i++)
                        if (this.htMap[t][i] == e) return {
                            x: t,
                            y: i,
                            elCell: e
                        };
                return {
                    x: 0,
                    y: 0,
                    elCell: e
                }
            },
            _applyTableTemplate: function(e, t) {
                e && (this._clearAllTableStyles(e), this._doApplyTableTemplate(e, nhn.husky.SE2M_TableTemplate[t], !1), e.setAttribute(this.ATTR_TBL_TEMPLATE, t))
            },
            _clearAllTableStyles: function(e) {
                e.removeAttribute("border"), e.removeAttribute("cellPadding"), e.removeAttribute("cellSpacing"), e.style.padding = "", e.style.border = "", e.style.backgroundColor = "", e.style.color = "";
                for (var t = jindo.$$(">TBODY>TR>TD", e, {
                        oneTimeOffCache: !0
                    }), i = 0, s = t.length; i < s; i++) t[i].style.padding = "", t[i].style.border = "", t[i].style.backgroundColor = "", t[i].style.color = "";
                var o = jindo.$$(">TBODY>TR>TH", e, {
                    oneTimeOffCache: !0
                });
                for (i = 0, s = o.length; i < s; i++) o[i].style.padding = "", o[i].style.border = "", o[i].style.backgroundColor = "", o[i].style.color = ""
            },
            _hideTableTemplate: function(e) {
                e.getAttribute(this.ATTR_TBL_TEMPLATE) && this._doApplyTableTemplate(e, nhn.husky.SE2M_TableTemplate[this.parseIntOr0(e.getAttribute(this.ATTR_TBL_TEMPLATE))], !0)
            },
            _showTableTemplate: function(e) {
                e.getAttribute(this.ATTR_TBL_TEMPLATE) && this._doApplyTableTemplate(e, nhn.husky.SE2M_TableTemplate[this.parseIntOr0(e.getAttribute(this.ATTR_TBL_TEMPLATE))], !1)
            },
            _doApplyTableTemplate: function(e, t, i) {
                var s, o = t.htTableProperty,
                    n = t.htTableStyle,
                    l = t.ht1stRowStyle,
                    r = t.ht1stColumnStyle,
                    a = t.aRowStyle;
                o && this._copyAttributesTo(e, o, i), n && this._copyStylesTo(e, n, i);
                var h, _, p = jindo.$$(">TBODY>TR", e, {
                        oneTimeOffCache: !0
                    }),
                    d = 0;
                if (l) {
                    d = 1;
                    for (var c = 0, E = p[0].childNodes.length; c < E; c++)(s = p[0].childNodes[c]).tagName && s.tagName.match(/^TD|TH$/) && this._copyStylesTo(s, l, i)
                }
                if (r)
                    for (var T = l ? 1 : 0, A = p.length; T < A;) h = 1, (_ = p[T].firstChild) && _.tagName.match(/^TD|TH$/) && (h = parseInt(_.getAttribute("rowSpan"), 10) || 1, this._copyStylesTo(_, r, i)), T += h;
                if (a) {
                    var S = a.length;
                    for (T = d, A = p.length; T < A; T++)
                        for (c = 0, E = p[T].childNodes.length; c < E; c++)(s = p[T].childNodes[c]).tagName && s.tagName.match(/^TD|TH$/) && this._copyStylesTo(s, a[(T + d) % S], i)
                }
            },
            _copyAttributesTo: function(e, t, i) {
                var s;
                for (var o in t) Object.prototype.hasOwnProperty.call(t, o) && (i ? e[o] && ((s = document.createElement(e.tagName))[o] = t[o], s[o] == e[o] && e.removeAttribute(o)) : ((s = document.createElement(e.tagName)).style[o] = "", e[o] && e.style[o] != s.style[o] || e.setAttribute(o, t[o])))
            },
            _copyStylesTo: function(e, t, i) {
                var s;
                for (var o in t) Object.prototype.hasOwnProperty.call(t, o) && (i ? e.style[o] && ((s = document.createElement(e.tagName)).style[o] = t[o], s.style[o] == e.style[o] && (e.style[o] = "")) : ((s = document.createElement(e.tagName)).style[o] = "", e.style[o] && e.style[o] != s.style[o] && !o.match(/^border/) || (e.style[o] = t[o])))
            },
            _hideResizer: function() {
                this.elResizeGrid.style.display = "none"
            },
            _showResizer: function() {
                this.elResizeGrid.style.display = "block"
            },
            _setResizerSize: function(e, t) {
                this.elResizeGrid.style.width = e + "px", this.elResizeGrid.style.height = t + "px"
            },
            parseBorder: function(e, t) {
                if ("none" == t) return 0;
                var i = parseInt(e, 10);
                return isNaN(i) && "string" == typeof e ? 1 : i
            },
            parseIntOr0: function(e) {
                return e = parseInt(e, 10), isNaN(e) ? 0 : e
            },
            _getTRCloneWithAllTD: function(e) {
                for (var t, i, s = this.htMap[0][e].parentNode.cloneNode(!1), o = 0, n = this.htMap.length; o < n; o++) t = this.htMap[o][e], this._rxCellNames.test(t.tagName) && ((i = this._shallowCloneTD(t)).setAttribute("rowSpan", 1), i.setAttribute("colSpan", 1), i.style.width = "", i.style.height = "", s.insertBefore(i, null));
                return s
            },
            _shallowCloneTD: function(e) {
                var t = e.cloneNode(!1);
                return t.innerHTML = this.sEmptyTDSrc, t
            },
            _isValidTable: function(e) {
                if (!e || !e.tagName || "TABLE" != e.tagName) return !1;
                this.htMap = this._getCellMapping(e);
                var t = this.htMap.length;
                if (t < 1) return !1;
                var i = this.htMap[0].length;
                if (i < 1) return !1;
                for (var s = 1; s < t; s++) {
                    if (this.htMap[s].length != i || !this.htMap[s][i - 1]) return !1;
                    for (var o = 0; o < i; o++)
                        if (!this.htMap[s] || !this.htMap[s][o]) return !1
                }
                return !0
            },
            addCSSClass: function(e, t) {
                var i = this.oApp.getWYSIWYGDocument();
                if (i.styleSheets[0] && i.styleSheets[0].addRule) i.styleSheets[0].addRule("." + e, t);
                else {
                    var s = i.getElementsByTagName("HEAD")[0],
                        o = i.createElement("STYLE");
                    s.appendChild(o), o.sheet.insertRule("." + e + " { " + t + " }", 0)
                }
            },
            _removeEmptyTextNode_IE: function(e) {
                var t = e.nextSibling;
                t && 3 == t.nodeType && !/\S/.test(t.nodeValue) && e.parentNode.removeChild(t)
            }
        })
    },
    62: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_SE2M_BGColor$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.SE2M_BGColor, {
            $ON_TOGGLE_BGCOLOR_LAYER: function() {
                this.oApp.exec("TOGGLE_TOOLBAR_ACTIVE_LAYER", [this.elDropdownLayer, null, "BGCOLOR_LAYER_SHOWN", [], "BGCOLOR_LAYER_HIDDEN", []]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["bgcolor"])
            },
            $ON_BGCOLOR_LAYER_SHOWN: function() {
                this.oApp.exec("SELECT_UI", ["BGColorB"]), this.oApp.exec("SHOW_COLOR_PALETTE", ["APPLY_BGCOLOR", this.elPaletteHolder])
            },
            $ON_BGCOLOR_LAYER_HIDDEN: function() {
                this.oApp.exec("DESELECT_UI", ["BGColorB"]), this.oApp.exec("RESET_COLOR_PALETTE", [])
            },
            $ON_EVENT_APPLY_BGCOLOR: function(e) {
                for (var t, i, s = e.element;
                    "SPAN" == s.tagName;) s = s.parentNode;
                "BUTTON" == s.tagName && (t = s.style.backgroundColor, i = s.style.color, this.oApp.exec("APPLY_BGCOLOR", [t, i]))
            },
            $ON_APPLY_LAST_USED_BGCOLOR: function() {
                this.oApp.exec("APPLY_BGCOLOR", [this.sLastUsedColor]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["bgcolor"])
            },
            $ON_APPLY_BGCOLOR: function(e, t) {
                if (this.rxColorPattern.test(e)) {
                    this._setLastUsedBGColor(e);
                    var i = {
                        backgroundColor: e
                    };
                    t && (i.color = t), this.oApp.exec("SET_WYSIWYG_STYLE", [i]), this.oApp.exec("HIDE_ACTIVE_LAYER")
                } else alert(this.oApp.$MSG("SE_Color.invalidColorCode"))
            }
        })
    },
    63: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_SE2M_FontColor$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.SE2M_FontColor, {
            $ON_TOGGLE_FONTCOLOR_LAYER: function() {
                this.oApp.exec("TOGGLE_TOOLBAR_ACTIVE_LAYER", [this.elDropdownLayer, null, "FONTCOLOR_LAYER_SHOWN", [], "FONTCOLOR_LAYER_HIDDEN", []]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["fontcolor"])
            },
            $ON_FONTCOLOR_LAYER_SHOWN: function() {
                this.oApp.exec("SELECT_UI", ["fontColorB"]), this.oApp.exec("SHOW_COLOR_PALETTE", ["APPLY_FONTCOLOR", this.elPaletteHolder])
            },
            $ON_FONTCOLOR_LAYER_HIDDEN: function() {
                this.oApp.exec("DESELECT_UI", ["fontColorB"]), this.oApp.exec("RESET_COLOR_PALETTE", [])
            },
            $ON_APPLY_LAST_USED_FONTCOLOR: function() {
                this.oApp.exec("APPLY_FONTCOLOR", [this.sLastUsedColor]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["fontcolor"])
            },
            $ON_APPLY_FONTCOLOR: function(e) {
                this.rxColorPattern.test(e) ? (this._setLastUsedFontColor(e), this.oApp.exec("SET_WYSIWYG_STYLE", [{
                    color: e
                }]), this.oApp.exec("HIDE_ACTIVE_LAYER")) : alert(this.oApp.$MSG("SE_FontColor.invalidColorCode"))
            }
        })
    },
    64: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_SE2M_Hyperlink$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.SE2M_Hyperlink, {
            $ON_TOGGLE_HYPERLINK_LAYER: function() {
                this.bLayerShown || (this.oApp.exec("IE_FOCUS", []), this.oSelection = this.oApp.getSelection()), this.oApp.delayedExec("TOGGLE_TOOLBAR_ACTIVE_LAYER", [this.oHyperlinkLayer, null, "MSG_HYPERLINK_LAYER_SHOWN", [], "MSG_HYPERLINK_LAYER_HIDDEN", [""]], 0), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["hyperlink"])
            },
            $ON_MSG_HYPERLINK_LAYER_SHOWN: function() {
                this.bLayerShown = !0;
                var e = this.oSelection.findAncestorByTagName("A");
                if ((e = e || this._getSelectedNode()) && !this.oSelection.collapsed) {
                    this.oSelection.selectNode(e), this.oSelection.select();
                    try {
                        var t = e.getAttribute("href");
                        this.oLinkInput.value = t && -1 == t.indexOf("#") ? t : "http://"
                    } catch (i) {
                        this.oLinkInput.value = "http://"
                    }
                    this.bModify = !0
                } else this.oLinkInput.value = "http://", this.bModify = !1;
                this.oApp.delayedExec("SELECT_UI", ["hyperlink"], 0), this.oLinkInput.focus(), this.oLinkInput.value = this.oLinkInput.value, this.oLinkInput.select()
            },
            $ON_MSG_HYPERLINK_LAYER_HIDDEN: function() {
                this.bLayerShown = !1, this.oApp.exec("DESELECT_UI", ["hyperlink"])
            },
            _validateTarget: function() {
                var e = jindo.$Agent().navigator(),
                    t = !0;
                return e.ie && jindo.$A(this.oSelection.getNodes(!0)).forEach(function(e) {
                    e && 1 == e.nodeType && "iframe" == e.tagName.toLowerCase() && "db" == e.getAttribute("s_type").toLowerCase() && (t = !1, jindo.$A.Break()), jindo.$A.Continue()
                }, this), t
            },
            $ON_APPLY_HYPERLINK: function() {
                if (this._validateTarget()) {
                    var e = this.oLinkInput.value;
                    /^((http|https|ftp|mailto):(?:\/\/)?)/.test(e) || (e = "http://" + e), e = e.replace(/\s+$/, "");
                    var t = jindo.$Agent().navigator(),
                        i = "";
                    if (this.oApp.exec("IE_FOCUS", []), t.ie && (i = '<span style="text-decoration:none;">&nbsp;</span>'), this._validateURL(e)) {
                        var s, o = "_self";
                        if (this.oApp.exec("RECORD_UNDO_BEFORE_ACTION", ["HYPERLINK", {
                                sSaveTarget: this.bModify ? "A" : null
                            }]), this.oSelection.collapsed) {
                            var n = "<a href='" + e + "' target=" + o + ">" + nhn.husky.SE2M_Utils.replaceSpecialChar(e) + "</a>" + i;
                            this.oSelection.pasteHTML(n), s = this.oSelection.placeStringBookmark()
                        } else {
                            s = this.oSelection.placeStringBookmark(), this.oSelection.select(), !t.ie || 8 !== t.version && 8 !== t.nativeVersion || (this.oApp.exec("IE_FOCUS", []), this.oSelection.moveToBookmark(s), this.oSelection.select());
                            var l = Math.ceil(1e4 * Math.random());
                            if ("" == e) this.oApp.exec("EXECCOMMAND", ["unlink"]);
                            else if (this._isExceptional()) {
                                this.oApp.exec("EXECCOMMAND", ["unlink", !1, "", {
                                    bDontAddUndoHistory: !0
                                }]);
                                var r = "<a href='" + e + "' target=" + o + ">";
                                jindo.$A(this.oSelection.getNodes(!0)).forEach(function(e) {
                                    var t = this.oApp.getEmptySelection();
                                    3 === e.nodeType ? (t.selectNode(e), t.pasteHTML(r + e.nodeValue + "</a>")) : 1 === e.nodeType && "IMG" === e.tagName && (t.selectNode(e), t.pasteHTML(r + jindo.$Element(e).outerHTML() + "</a>"))
                                }, this)
                            } else this.oApp.exec("EXECCOMMAND", ["createLink", !1, this.sATagMarker + l + encodeURIComponent(e), {
                                bDontAddUndoHistory: !0
                            }]);
                            for (var a, h = this.oApp.getWYSIWYGDocument().body.getElementsByTagName("A"), _ = h.length, p = new RegExp(this.sRXATagMarker + l, "gi"), d = 0; d < _; d++) {
                                a = h[d];
                                var c = "";
                                try {
                                    c = a.getAttribute("href")
                                } catch (S) {}
                                if (c && c.match(p)) {
                                    var E = c.replace(p, ""),
                                        T = decodeURIComponent(E);
                                    if (t.ie) jindo.$Element(a).attr({
                                        href: T,
                                        target: o
                                    });
                                    else {
                                        var A = jindo.$Element(a).html();
                                        jindo.$Element(a).attr({
                                            href: T,
                                            target: o
                                        }), this._validateURL(A) && jindo.$Element(a).html(jindo.$Element(a).attr("href"))
                                    }
                                }
                            }
                        }
                        this.oApp.exec("HIDE_ACTIVE_LAYER"), setTimeout(jindo.$Fn(function() {
                            var e = this.oApp.getEmptySelection();
                            e.moveToBookmark(s), e.collapseToEnd(), e.select(), e.removeStringBookmark(s), this.oApp.exec("FOCUS"), this.oApp.exec("RECORD_UNDO_AFTER_ACTION", ["HYPERLINK", {
                                sSaveTarget: this.bModify ? "A" : null
                            }])
                        }, this).bind(), 17)
                    } else alert(this.oApp.$MSG("SE_Hyperlink.invalidURL")), this.oLinkInput.focus()
                } else alert(this.oApp.$MSG("SE_Hyperlink.invalidTarget"))
            },
            _isExceptional: function() {
                var e = jindo.$Agent().navigator();
                return !!e.ie && (!(!this.oApp.getWYSIWYGDocument().selection || "None" !== this.oApp.getWYSIWYGDocument().selection.type || !jindo.$A(this.oSelection.getNodes()).some(function(e) {
                    if (1 === e.nodeType && "IMG" === e.tagName) return !0
                }, this)) || !(8 < e.nativeVersion) && !!jindo.$A(this.oSelection.getTextNodes()).some(function(e) {
                    if (1 <= e.nodeValue.indexOf("@")) return !0
                }, this))
            },
            _getSelectedNode: function() {
                for (var e = this.oSelection.getNodes(), t = 0; t < e.length; t++)
                    if (e[t].tagName && "A" == e[t].tagName) return e[t]
            },
            _validateURL: function(e) {
                if (!e) return !1;
                try {
                    var t = e.split("?");
                    t[0] = t[0].replace(/%[a-z0-9]{2}/gi, "U"), decodeURIComponent(t[0])
                } catch (i) {
                    return !1
                }
                return /^(http|https|ftp|mailto):(\/\/)?(([-가-힣]|\w)+(?:[/.:@]([-가-힣]|\w)+)+)\/?(.*)?\s*$/i.test(e)
            }
        })
    },
    65: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_SE2M_LineHeightWithLayerUI$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.SE2M_LineHeightWithLayerUI, {
            _assignHTMLObjects: function(e) {
                this.oDropdownLayer = jindo.$$.getSingle("DIV.husky_se2m_lineHeight_layer", e), this.aLIOptions = jindo.$A(jindo.$$("LI", this.oDropdownLayer)).filter(function(e) {
                    return null !== e.firstChild
                })._array, this.oInput = jindo.$$.getSingle("INPUT", this.oDropdownLayer);
                var t = jindo.$$.getSingle(".husky_se2m_lineHeight_direct_input", this.oDropdownLayer);
                t = jindo.$$("BUTTON", t), this.oBtn_up = t[0], this.oBtn_down = t[1], this.oBtn_ok = t[2], this.oBtn_cancel = t[3]
            },
            $LOCAL_BEFORE_FIRST: function() {
                this._assignHTMLObjects(this.oApp.htOptions.elAppContainer), this.oApp.exec("SE2_ATTACH_HOVER_EVENTS", [this.aLIOptions]);
                for (var e = 0; e < this.aLIOptions.length; e++) this.oApp.registerBrowserEvent(this.aLIOptions[e], "click", "SET_LINEHEIGHT_FROM_LAYER_UI", [this._getLineHeightFromLI(this.aLIOptions[e])]);
                this.oApp.registerBrowserEvent(this.oBtn_up, "click", "SE2M_INC_LINEHEIGHT", []), this.oApp.registerBrowserEvent(this.oBtn_down, "click", "SE2M_DEC_LINEHEIGHT", []), this.oApp.registerBrowserEvent(this.oBtn_ok, "click", "SE2M_SET_LINEHEIGHT_FROM_DIRECT_INPUT", []), this.oApp.registerBrowserEvent(this.oBtn_cancel, "click", "SE2M_CANCEL_LINEHEIGHT", []), this.oApp.registerBrowserEvent(this.oInput, "keydown", "EVENT_SE2M_LINEHEIGHT_KEYDOWN")
            },
            $ON_EVENT_SE2M_LINEHEIGHT_KEYDOWN: function(e) {
                e.key().enter && (this.oApp.exec("SE2M_SET_LINEHEIGHT_FROM_DIRECT_INPUT"), e.stop())
            },
            $ON_SE2M_TOGGLE_LINEHEIGHT_LAYER: function() {
                this.oApp.exec("TOGGLE_TOOLBAR_ACTIVE_LAYER", [this.oDropdownLayer, null, "LINEHEIGHT_LAYER_SHOWN", [], "LINEHEIGHT_LAYER_HIDDEN", []]), this.oApp.exec("MSG_NOTIFY_CLICKCR", ["lineheight"])
            },
            $ON_SE2M_INC_LINEHEIGHT: function() {
                this.oInput.value = parseInt(this.oInput.value, 10) || this.MIN_LINE_HEIGHT, this.oInput.value++
            },
            $ON_SE2M_DEC_LINEHEIGHT: function() {
                this.oInput.value = parseInt(this.oInput.value, 10) || this.MIN_LINE_HEIGHT, this.oInput.value > this.MIN_LINE_HEIGHT && this.oInput.value--
            },
            $ON_LINEHEIGHT_LAYER_SHOWN: function() {
                this.oApp.exec("SELECT_UI", ["lineHeight"]), this.oInitialSelection = this.oApp.getSelection();
                var e = this.oApp.getLineStyle("lineHeight");
                if (null != e && 0 !== e) {
                    this.oInput.value = (100 * e).toFixed(0);
                    var t = this._getMatchingLI(this.oInput.value + "%");
                    t && jindo.$Element(t.firstChild).addClass("active")
                } else this.oInput.value = ""
            },
            $ON_LINEHEIGHT_LAYER_HIDDEN: function() {
                this.oApp.exec("DESELECT_UI", ["lineHeight"]), this._clearOptionSelection()
            },
            $ON_SE2M_SET_LINEHEIGHT_FROM_DIRECT_INPUT: function() {
                var e = parseInt(this.oInput.value, 10),
                    t = e < this.MIN_LINE_HEIGHT ? this.MIN_LINE_HEIGHT : e;
                this._setLineHeightAndCloseLayer(t)
            },
            $ON_SET_LINEHEIGHT_FROM_LAYER_UI: function(e) {
                this._setLineHeightAndCloseLayer(e)
            },
            $ON_SE2M_CANCEL_LINEHEIGHT: function() {
                this.oInitialSelection.select(), this.oApp.exec("HIDE_ACTIVE_LAYER")
            },
            _setLineHeightAndCloseLayer: function(e) {
                var t = parseInt(e, 10) / 100;
                0 < t ? this.oApp.exec("SET_LINE_STYLE", ["lineHeight", t]) : alert(this.oApp.$MSG("SE_LineHeight.invalidLineHeight")), this.oApp.exec("SE2M_TOGGLE_LINEHEIGHT_LAYER", []);
                var i = jindo.$Agent().navigator();
                (i.chrome || i.safari) && this.oApp.exec("FOCUS")
            },
            _getMatchingLI: function(e) {
                var t;
                e = e.toLowerCase();
                for (var i = 0; i < this.aLIOptions.length; i++)
                    if (t = this.aLIOptions[i], this._getLineHeightFromLI(t).toLowerCase() == e) return t;
                return null
            },
            _getLineHeightFromLI: function(e) {
                return e.firstChild.firstChild.innerHTML
            },
            _clearOptionSelection: function() {
                for (var e = 0; e < this.aLIOptions.length; e++) jindo.$Element(this.aLIOptions[e].firstChild).removeClass("active")
            }
        })
    },
    66: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_SE2M_QuickEditor_Common$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.SE2M_QuickEditor_Common, {
            setOpenType: function(e, t) {
                "undefined" != typeof this._environmentData && null != this._environmentData || (this._environmentData = {}), "undefined" != typeof this._environmentData[e] && null != this._environmentData[e] || (this._environmentData[e] = {}), "undefined" != typeof this._environmentData[e].isOpen && null != this._environmentData[e].isOpen || (this._environmentData[e].isOpen = !0), this._environmentData[e].isOpen = t
            },
            $ON_OPEN_QE_LAYER: function(e, t, i) {
                if (0 < this.waHotkeys.length() && !this.waHotkeyLayers.has(t)) {
                    var s;
                    this.waHotkeyLayers.push(t);
                    for (var o = 0, n = this.waHotkeys.length(); o < n; o++) s = this.waHotkeys.get(o), this.oApp.exec("ADD_HOTKEY", [s[0], s[1], s[2], t])
                }
                var l = i;
                l && (this.targetEle = e, this.currentEle = t, this.layer_show(l, e))
            },
            $ON_CLOSE_QE_LAYER: function(e) {
                this.currentEle && (this.oApp.exec("CLOSE_SUB_LAYER_QE"), this.layer_hide(e))
            },
            $LOCAL_BEFORE_FIRST: function(e) {
                if (!e.match(/OPEN_QE_LAYER/)) return this.oApp.acceptLocalBeforeFirstAgain(this, !0), !!e.match(/REGISTER_HOTKEY/);
                this.woEditor = jindo.$Element(this.oApp.elEditingAreaContainer), this.woStandard = jindo.$Element(this.oApp.htOptions.elAppContainer).offset(), this._qe_wrap = jindo.$$.getSingle("DIV.quick_wrap", this.oApp.htOptions.elAppContainer);
                var i = this;
                new jindo.DragArea(this._qe_wrap, {
                    sClassName: "q_dragable",
                    bFlowOut: !1,
                    nThreshold: 1
                }).attach({
                    beforeDrag: function(e) {
                        e.elFlowOut = e.elArea.parentNode
                    },
                    dragStart: function(e) {
                        jindo.$Element(e.elDrag).hasClass("se2_qmax") || (e.elDrag = e.elDrag.parentNode), i.oApp.exec("SHOW_EDITING_AREA_COVER")
                    },
                    dragEnd: function(e) {
                        i.changeFixedMode(), i._in_event = !1;
                        var t = jindo.$Element(e.elDrag);
                        i._environmentData[i._currentType].position = [t.css("top"), t.css("left")], i.oApp.exec("HIDE_EDITING_AREA_COVER")
                    }
                });
                var t = jindo.$Fn(this.toggle, this).bind("img"),
                    s = jindo.$Fn(this.toggle, this).bind("table");
                jindo.$Fn(t, this).attach(jindo.$$.getSingle(".q_open_img_fold", this.oApp.htOptions.elAppContainer), "click"), jindo.$Fn(t, this).attach(jindo.$$.getSingle(".q_open_img_full", this.oApp.htOptions.elAppContainer), "click"), jindo.$Fn(s, this).attach(jindo.$$.getSingle(".q_open_table_fold", this.oApp.htOptions.elAppContainer), "click"), jindo.$Fn(s, this).attach(jindo.$$.getSingle(".q_open_table_full", this.oApp.htOptions.elAppContainer), "click")
            },
            toggle: function(e, t) {
                e = this._currentType, this.oApp.exec("CLOSE_QE_LAYER", [t]), "full" == this._environmentData[e].type ? this._environmentData[e].type = "fold" : this._environmentData[e].type = "full", this._environmentData && this._bUseConfig && jindo.$Ajax(this._sAddTextAjaxUrl, {
                    type: "jsonp",
                    onload: function() {}
                }).request({
                    text_key: "qeditor_fold",
                    text_data: "{table:'" + this._environmentData.table.type + "',img:'" + this._environmentData.img.type + "',review:'" + this._environmentData.review.type + "'}"
                }), this.oApp.exec("OPEN_QE_LAYER", [this.targetEle, this.currentEle, e]), this._in_event = !1, t.stop(jindo.$Event.CANCEL_DEFAULT)
            },
            positionCopy: function(e, t, i) {
                jindo.$Element(jindo.$$.getSingle("._" + i, this.currentEle)).css({
                    top: t,
                    left: e
                })
            },
            changeFixedMode: function() {
                this._environmentData[this._currentType].isFixed = !0
            },
            $ON_HIDE_ACTIVE_LAYER: function() {
                this.oApp.exec("CLOSE_QE_LAYER")
            },
            $ON_EVENT_EDITING_AREA_MOUSEDOWN: function(e) {
                this._currentType && !this._in_event && this._environmentData[this._currentType].isOpen && this.oApp.exec("CLOSE_QE_LAYER", [e]), this._in_event = !1
            },
            $ON_EVENT_EDITING_AREA_MOUSEWHEEL: function(e) {
                this._currentType && !this._in_event && this._environmentData[this._currentType].isOpen && this.oApp.exec("CLOSE_QE_LAYER", [e]), this._in_event = !1
            },
            get_type: function(e) {
                var t = e.tagName.toLowerCase();
                return this.waTableTagNames.has(t) ? "table" : "img" == t ? "img" : void 0
            },
            $ON_QE_IN_KEYUP: function() {
                this._in_event = !0
            },
            $ON_QE_IN_MOUSEDOWN: function() {
                this._in_event = !0
            },
            $ON_QE_IN_MOUSEWHEEL: function() {
                this._in_event = !0
            },
            layer_hide: function() {
                this.setOpenType(this._currentType, !1), jindo.$Element(jindo.$$.getSingle("._" + this._environmentData[this._currentType].type, this.currentEle)).hide()
            },
            lazy_common: function() {
                this.oApp.registerBrowserEvent(jindo.$(this._qe_wrap), "keyup", "QE_IN_KEYUP"), this.oApp.registerBrowserEvent(jindo.$(this._qe_wrap), "mousedown", "QE_IN_MOUSEDOWN"), this.oApp.registerBrowserEvent(jindo.$(this._qe_wrap), "mousewheel", "QE_IN_MOUSEWHEEL"), this.lazy_common = function() {}
            },
            layer_show: function(e, t) {
                this._currentType = e, this.setOpenType(this._currentType, !0);
                var i = jindo.$$.getSingle("._" + this._environmentData[this._currentType].type, this.currentEle);
                jindo.$Element(i).show().css(this.get_position_layer(t, i)), this.lazy_common()
            },
            get_position_layer: function(e, t) {
                if (!this.isCurrentFixed() || "fold" == this._environmentData[this._currentType].type) return this.calculateLayer(e, t);
                var i = this._environmentData[this._currentType].position,
                    s = parseInt(i[0], 10),
                    o = this.getAppPosition().h,
                    n = jindo.$Element(t).height();
                return s + n + this.nYGap > o && (s = o - n, this._environmentData[this._currentType].position[0] = s), {
                    top: s + "px",
                    left: i[1]
                }
            },
            isCurrentFixed: function() {
                return this._environmentData[this._currentType].isFixed
            },
            calculateLayer: function(e, t) {
                var i = this.getPositionInfo(e, t);
                return {
                    top: i.y + "px",
                    left: i.x + "px"
                }
            },
            getPositionInfo: function(e, t) {
                this.nYGap = jindo.$Agent().navigator().ie ? -16 : -18, this.nXGap = 1;
                var i = {},
                    s = this.getElementPosition(e, t),
                    o = this.getAppPosition(),
                    n = jindo.$Element(t).width(),
                    l = jindo.$Element(t).height();
                return s.x + n + this.nXGap > o.w ? i.x = o.w - n : i.x = s.x + this.nXGap, s.y + l + this.nYGap > o.h ? i.y = o.h - l - 2 : i.y = s.y + this.nYGap, {
                    x: i.x,
                    y: i.y
                }
            },
            getElementPosition: function(e, t) {
                var i, s, o, n, l, r;
                n = e ? (s = (i = jindo.$Element(e)).offset(), o = i.width(), i.height()) : (s = {
                    top: parseInt(t.style.top, 10) - this.nYGap,
                    left: parseInt(t.style.left, 10) - this.nXGap
                }, o = 0);
                var a = this.oApp.getWYSIWYGWindow();
                return r = "undefined" == typeof a.scrollX ? (l = a.document.documentElement.scrollLeft, a.document.documentElement.scrollTop) : (l = a.scrollX, a.scrollY), {
                    x: s.left - l + o,
                    y: s.top - r + n
                }
            },
            getAppPosition: function() {
                return {
                    w: this.woEditor.width(),
                    h: this.woEditor.height()
                }
            }
        })
    },
    67: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("hp_DialogLayerManager$Lazy.js"), nhn.husky.HuskyCore.mixin(nhn.husky.DialogLayerManager, {
            $ON_SHOW_DIALOG_LAYER: function(e, t) {
                if (t = t || {}, (e = jindo.$(e)) && !jindo.$A(this.aOpenedLayers).has(e)) {
                    var i;
                    this.oApp.exec("POSITION_DIALOG_LAYER", [e]), this.aOpenedLayers[this.aOpenedLayers.length] = e;
                    var s = jindo.$A(this.aMadeDraggable).indexOf(e); - 1 == s ? (i = new nhn.DraggableLayer(e, t), this.aMadeDraggable[this.aMadeDraggable.length] = e, this.aDraggableLayer[this.aDraggableLayer.length] = i) : (t && (i = this.aDraggableLayer[s]).setOptions(t), e.style.display = "block"), t.sOnShowMsg && this.oApp.exec(t.sOnShowMsg, t.sOnShowParam)
                }
            },
            $ON_HIDE_LAST_DIALOG_LAYER: function() {
                this.oApp.exec("HIDE_DIALOG_LAYER", [this.aOpenedLayers[this.aOpenedLayers.length - 1]])
            },
            $ON_HIDE_ALL_DIALOG_LAYER: function() {
                for (var e = this.aOpenedLayers.length - 1; 0 <= e; e--) this.oApp.exec("HIDE_DIALOG_LAYER", [this.aOpenedLayers[e]])
            },
            $ON_HIDE_DIALOG_LAYER: function(e) {
                (e = jindo.$(e)) && (e.style.display = "none"), this.aOpenedLayers = jindo.$A(this.aOpenedLayers).refuse(e).$value()
            },
            $ON_TOGGLE_DIALOG_LAYER: function(e, t) {
                jindo.$A(this.aOpenedLayers).indexOf(e) ? this.oApp.exec("SHOW_DIALOG_LAYER", [e, t]) : this.oApp.exec("HIDE_DIALOG_LAYER", [e])
            },
            $ON_SET_DIALOG_LAYER_POSITION: function(e, t, i) {
                e.style.top = t, e.style.left = i
            }
        })
    },
    68: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("N_FindReplace.js"), nhn.FindReplace = jindo.$Class({
            sKeyword: "",
            window: null,
            document: null,
            bBrowserSupported: !1,
            _bLGDevice: !1,
            bEOC: !1,
            $init: function(e) {
                if (this.sInlineContainer = "SPAN|B|U|I|S|STRIKE", this.rxInlineContainer = new RegExp("^(" + this.sInlineContainer + ")$"), this.window = e, this.document = this.window.document, this.document.domain != this.document.location.hostname) {
                    var t = jindo.$Agent().navigator();
                    if (t.firefox && t.version < 3) return this.bBrowserSupported = !1, void(this.find = function() {
                        return 3
                    })
                }
                this._bLGDevice = -1 < navigator.userAgent.indexOf("LG-"), this.bBrowserSupported = !0
            },
            find: function(e, t, i, s) {
                return this._bLGDevice || this.window.focus(), e ? (this.bEOC = !1, this.findNext(e, t, i, s) ? 0 : (this.bEOC = !0, this.findNew(e, t, i, s) ? 0 : 1)) : 2
            },
            findNew: function(e, t, i, s) {
                return this.findReset(), this.findNext(e, t, i, s)
            },
            findNext: function(e, t, i, s) {
                var o;
                if (t = t || !1, s = s || !1, i = i || !1, this.window.find) {
                    return "false" === this.document.body.contentEditable ? window.find(e, t, i, !1, s) : this.window.find(e, t, i, !1, s)
                }
                if (this.document.body.createTextRange) try {
                    var n = 0;
                    return i && (n += 1), s && (n += 2), t && (n += 4), this.window.focus(), this.document.selection ? (this._range = this.document.selection.createRangeCollection().item(0), this._range.collapse(!1)) : this._range ? this._range.collapse(!1) : this._range = this.document.body.createTextRange(), o = this._range.findText(e, 1, n), this._range.select(), o
                } catch (l) {
                    return !1
                }
                return !1
            },
            findReset: function() {
                this.window.find ? this.window.getSelection().removeAllRanges() : this.document.body.createTextRange && (this._range = this.document.body.createTextRange(), this._range.collapse(!0), this._range.select())
            },
            replace: function(e, t, i, s, o) {
                return this._replace(e, t, i, s, o)
            },
            _replace: function(e, t, i, s, o, n) {
                if (!e) return 4;
                (n = n || new nhn.HuskyRange(this.window)).setFromSelection(), i = i || !1;
                var l = n.toString();
                return (i ? l == e : l.toLowerCase() == e.toLowerCase()) ? ("function" == typeof t ? n = t(n) : n.pasteText(t), n.select(), this.find(e, i, s, o)) : this.find(e, i, s, o) + 2
            },
            replaceAll: function(e, t, i, s) {
                if (!e) return -1;
                var o, n = 0,
                    l = this.window;
                if (0 !== this.find(e, i, !1, s)) return n;
                var r = new nhn.HuskyRange(this.window);
                r.setFromSelection(), r.collapseToStart();
                var a = this.window.document.createElement("SPAN");
                a.innerHTML = unescape("%uFEFF"), r.insertNode(a), r.select();
                var h = r.placeStringBookmark();
                for (this.bEOC = !1; !this.bEOC;) 0 != (o = this._replace(e, t, i, !1, s, r)) && 1 != o || n++;
                var _;
                for (o = 0, this.bEOC = !1; _ = void 0, (_ = new nhn.HuskyRange(l)).setFromSelection(), r.moveToBookmark(h), 1 == r.compareBoundaryPoints(nhn.W3CDOMRange.START_TO_END, _) && 0 == o && !this.bEOC;) 0 != (o = this._replace(e, t, i, !1, s, r)) && 1 != o || n++;
                return r.moveToBookmark(h), r.deleteContents(), r.removeStringBookmark(h), setTimeout(function() {
                    a && a.parentNode && a.parentNode.removeChild(a)
                }, 0), n
            },
            _isBlankTextNode: function(e) {
                return 3 == e.nodeType && "" == e.nodeValue
            },
            _getNextNode: function(e, t) {
                if (!e || "BODY" == e.tagName) return {
                    elNextNode: null,
                    bDisconnected: !1
                };
                if (e.nextSibling) {
                    for (e = e.nextSibling; e.firstChild;) e.tagName && !this.rxInlineContainer.test(e.tagName) && (t = !0), e = e.firstChild;
                    return {
                        elNextNode: e,
                        bDisconnected: t
                    }
                }
                return this._getNextNode(nhn.DOMFix.parentNode(e), t)
            },
            _getNextTextNode: function(e, t) {
                for (var i; e = (i = this._getNextNode(e, t)).elNextNode, t = i.bDisconnected, e && 3 != e.nodeType && !this.rxInlineContainer.test(e.tagName) && (t = !0), e && (3 != e.nodeType || this._isBlankTextNode(e)););
                return {
                    elNextText: e,
                    bDisconnected: t
                }
            },
            _getFirstTextNode: function() {
                for (var e = this.document.body.firstChild; e && e.firstChild;) e = e.firstChild;
                if (!e) return null;
                3 == e.nodeType && !this._isBlankTextNode(e) || (e = this._getNextTextNode(e, !1).elNextText);
                return e
            },
            _addToTextMap: function(e, t, i, s) {
                for (var o = t[s].length, n = 0, l = e.nodeValue.length; n < l; n++) i[s][o + n] = [e, n];
                t[s] += e.nodeValue
            },
            _createTextMap: function() {
                for (var e = [], t = [], i = -1, s = this._getFirstTextNode(), o = {
                        elNextText: s,
                        bDisconnected: !0
                    }; s;) o.bDisconnected && (e[++i] = "", t[i] = []), this._addToTextMap(o.elNextText, e, t, i), s = (o = this._getNextTextNode(s, !1)).elNextText;
                return {
                    aTexts: e,
                    aElTexts: t
                }
            },
            replaceAll_js: function(e, t, i, s) {
                try {
                    for (var o = this._createTextMap(), n = o.aTexts, l = o.aElTexts, r = 0, a = e.length, h = 0, _ = n.length; h < _; h++)
                        for (var p = n[h], d = p.length - a; 0 <= d; d--) {
                            var c = p.substring(d, d + a);
                            if (!(s && 0 < d && p.charAt(d - 1).match(/[a-zA-Z가-힣]/)) && c == e) {
                                r++;
                                var E, T, A = new nhn.HuskyRange(this.window);
                                T = d + a < l[h].length ? (E = l[h][d + a][0], l[h][d + a][1]) : (E = l[h][d + a - 1][0], l[h][d + a - 1][1] + 1), A.setEnd(E, T, !0, !0), A.setStart(l[h][d][0], l[h][d][1], !0), "function" == typeof t ? A = t(A) : A.pasteText(t), d -= a
                            }
                        }
                    return r
                } catch (S) {
                    return r
                }
            }
        })
    },
    69: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("SE2M_TableTemplate.js"), nhn.husky.SE2M_TableTemplate = [{}, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "1"
            },
            htTableStyle: {
                backgroundColor: "#c7c7c7"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#ffffff",
                color: "#666666"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "1"
            },
            htTableStyle: {
                backgroundColor: "#c7c7c7"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#ffffff",
                color: "#666666"
            }, {
                padding: "3px 4px 2px",
                backgroundColor: "#f3f3f3",
                color: "#666666"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "0"
            },
            htTableStyle: {
                backgroundColor: "#ffffff",
                borderTop: "1px solid #c7c7c7"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                borderBottom: "1px solid #c7c7c7",
                backgroundColor: "#ffffff",
                color: "#666666"
            }, {
                padding: "3px 4px 2px",
                borderBottom: "1px solid #c7c7c7",
                backgroundColor: "#f3f3f3",
                color: "#666666"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "0"
            },
            htTableStyle: {
                border: "1px solid #c7c7c7"
            },
            ht1stRowStyle: {
                padding: "3px 4px 2px",
                backgroundColor: "#f3f3f3",
                color: "#666666",
                borderRight: "1px solid #e7e7e7",
                textAlign: "left",
                fontWeight: "normal"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#ffffff",
                borderTop: "1px solid #e7e7e7",
                borderRight: "1px solid #e7e7e7",
                color: "#666666"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "1"
            },
            htTableStyle: {
                backgroundColor: "#c7c7c7"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#f8f8f8",
                color: "#666666"
            }, {
                padding: "3px 4px 2px",
                backgroundColor: "#ebebeb",
                color: "#666666"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "0"
            },
            ht1stRowStyle: {
                padding: "3px 4px 2px",
                borderTop: "1px solid #000000",
                borderBottom: "1px solid #000000",
                backgroundColor: "#333333",
                color: "#ffffff",
                textAlign: "left",
                fontWeight: "normal"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                borderBottom: "1px solid #ebebeb",
                backgroundColor: "#ffffff",
                color: "#666666"
            }, {
                padding: "3px 4px 2px",
                borderBottom: "1px solid #ebebeb",
                backgroundColor: "#f8f8f8",
                color: "#666666"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "1"
            },
            htTableStyle: {
                backgroundColor: "#c7c7c7"
            },
            ht1stRowStyle: {
                padding: "3px 4px 2px",
                backgroundColor: "#333333",
                color: "#ffffff",
                textAlign: "left",
                fontWeight: "normal"
            },
            ht1stColumnStyle: {
                padding: "3px 4px 2px",
                backgroundColor: "#f8f8f8",
                color: "#666666",
                textAlign: "left",
                fontWeight: "normal"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#ffffff",
                color: "#666666"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "1"
            },
            htTableStyle: {
                backgroundColor: "#c7c7c7"
            },
            ht1stColumnStyle: {
                padding: "3px 4px 2px",
                backgroundColor: "#333333",
                color: "#ffffff",
                textAlign: "left",
                fontWeight: "normal"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#ffffff",
                color: "#666666"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "1"
            },
            htTableStyle: {
                backgroundColor: "#a6bcd1"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#ffffff",
                color: "#3d76ab"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "1"
            },
            htTableStyle: {
                backgroundColor: "#a6bcd1"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#ffffff",
                color: "#3d76ab"
            }, {
                padding: "3px 4px 2px",
                backgroundColor: "#f6f8fa",
                color: "#3d76ab"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "0"
            },
            htTableStyle: {
                backgroundColor: "#ffffff",
                borderTop: "1px solid #a6bcd1"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                borderBottom: "1px solid #a6bcd1",
                backgroundColor: "#ffffff",
                color: "#3d76ab"
            }, {
                padding: "3px 4px 2px",
                borderBottom: "1px solid #a6bcd1",
                backgroundColor: "#f6f8fa",
                color: "#3d76ab"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "0"
            },
            htTableStyle: {
                border: "1px solid #a6bcd1"
            },
            ht1stRowStyle: {
                padding: "3px 4px 2px",
                backgroundColor: "#f6f8fa",
                color: "#3d76ab",
                borderRight: "1px solid #e1eef7",
                textAlign: "left",
                fontWeight: "normal"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#ffffff",
                borderTop: "1px solid #e1eef7",
                borderRight: "1px solid #e1eef7",
                color: "#3d76ab"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "1"
            },
            htTableStyle: {
                backgroundColor: "#a6bcd1"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#fafbfc",
                color: "#3d76ab"
            }, {
                padding: "3px 4px 2px",
                backgroundColor: "#e6ecf2",
                color: "#3d76ab"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "0"
            },
            ht1stRowStyle: {
                padding: "3px 4px 2px",
                borderTop: "1px solid #466997",
                borderBottom: "1px solid #466997",
                backgroundColor: "#6284ab",
                color: "#ffffff",
                textAlign: "left",
                fontWeight: "normal"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                borderBottom: "1px solid #ebebeb",
                backgroundColor: "#ffffff",
                color: "#3d76ab"
            }, {
                padding: "3px 4px 2px",
                borderBottom: "1px solid #ebebeb",
                backgroundColor: "#f6f8fa",
                color: "#3d76ab"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "1"
            },
            htTableStyle: {
                backgroundColor: "#a6bcd1"
            },
            ht1stRowStyle: {
                padding: "3px 4px 2px",
                backgroundColor: "#6284ab",
                color: "#ffffff",
                textAlign: "left",
                fontWeight: "normal"
            },
            ht1stColumnStyle: {
                padding: "3px 4px 2px",
                backgroundColor: "#f6f8fa",
                color: "#3d76ab",
                textAlign: "left",
                fontWeight: "normal"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#ffffff",
                color: "#3d76ab"
            }]
        }, {
            htTableProperty: {
                border: "0",
                cellPadding: "0",
                cellSpacing: "1"
            },
            htTableStyle: {
                backgroundColor: "#a6bcd1"
            },
            ht1stColumnStyle: {
                padding: "3px 4px 2px",
                backgroundColor: "#6284ab",
                color: "#ffffff",
                textAlign: "left",
                fontWeight: "normal"
            },
            aRowStyle: [{
                padding: "3px 4px 2px",
                backgroundColor: "#ffffff",
                color: "#3d76ab"
            }]
        }]
    },
    70: function(e, t) {
        nhn.husky.HuskyCore.addLoadedFile("N_DraggableLayer.js"), nhn.DraggableLayer = jindo.$Class({
            $init: function(e, t) {
                this.elLayer = e, this.setOptions(t), this.elHandle = this.oOptions.elHandle, e.style.display = "block", e.style.position = "absolute", e.style.zIndex = "9999", this.aBasePosition = this.getBaseOffset(e);
                var i = this.toInt(jindo.$Element(e).offset().top) - this.aBasePosition.top,
                    s = this.toInt(jindo.$Element(e).offset().left) - this.aBasePosition.left,
                    o = this._correctXY({
                        x: s,
                        y: i
                    });
                e.style.top = o.y + "px", e.style.left = o.x + "px", this.$FnMouseDown = jindo.$Fn(jindo.$Fn(this._mousedown, this).bind(e), this), this.$FnMouseMove = jindo.$Fn(jindo.$Fn(this._mousemove, this).bind(e), this), this.$FnMouseUp = jindo.$Fn(jindo.$Fn(this._mouseup, this).bind(e), this), this.$FnMouseDown.attach(this.elHandle, "mousedown"), this.elHandle.ondragstart = new Function("return false"), this.elHandle.onselectstart = new Function("return false")
            },
            _mousedown: function(e, t) {
                "INPUT" != t.element.tagName && (this.oOptions.fnOnDragStart(), this.MouseOffsetY = t.pos().clientY - this.toInt(e.style.top) - this.aBasePosition.top, this.MouseOffsetX = t.pos().clientX - this.toInt(e.style.left) - this.aBasePosition.left, this.$FnMouseMove.attach(e.ownerDocument, "mousemove"), this.$FnMouseUp.attach(e.ownerDocument, "mouseup"), this.elHandle.style.cursor = "move")
            },
            _mousemove: function(e, t) {
                var i = t.pos().clientY - this.MouseOffsetY - this.aBasePosition.top,
                    s = t.pos().clientX - this.MouseOffsetX - this.aBasePosition.left,
                    o = this._correctXY({
                        x: s,
                        y: i
                    });
                e.style.top = o.y + "px", e.style.left = o.x + "px"
            },
            _mouseup: function(e) {
                this.oOptions.fnOnDragEnd(), this.$FnMouseMove.detach(e.ownerDocument, "mousemove"), this.$FnMouseUp.detach(e.ownerDocument, "mouseup"), this.elHandle.style.cursor = ""
            },
            _correctXY: function(e) {
                var t = e.x,
                    i = e.y;
                return i < this.oOptions.nMinY && (i = this.oOptions.nMinY), i > this.oOptions.nMaxY && (i = this.oOptions.nMaxY), t < this.oOptions.nMinX && (t = this.oOptions.nMinX), t > this.oOptions.nMaxX && (t = this.oOptions.nMaxX), {
                    x: t,
                    y: i
                }
            },
            toInt: function(e) {
                return parseInt(e) || 0
            },
            findNonStatic: function(e) {
                return e ? "BODY" == e.tagName ? e : jindo.$Element(e).css("position").match(/absolute|relative/i) ? e : this.findNonStatic(e.offsetParent) : null
            },
            getBaseOffset: function(e) {
                var t = this.findNonStatic(e.offsetParent) || e.ownerDocument.body,
                    i = jindo.$Element(t).offset();
                return {
                    top: i.top,
                    left: i.left
                }
            },
            setOptions: function(e) {
                this.oOptions = e || {}, this.oOptions.bModal = this.oOptions.bModal || !1, this.oOptions.elHandle = this.oOptions.elHandle || this.elLayer, this.oOptions.nMinX = this.oOptions.nMinX || -999999, this.oOptions.nMinY = this.oOptions.nMinY || -999999, this.oOptions.nMaxX = this.oOptions.nMaxX || 999999, this.oOptions.nMaxY = this.oOptions.nMaxY || 999999, this.oOptions.fnOnDragStart = this.oOptions.fnOnDragStart || function() {}, this.oOptions.fnOnDragEnd = this.oOptions.fnOnDragEnd || function() {}
            }
        })
    }
});