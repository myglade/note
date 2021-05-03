/*! Copyright (C) NAVER corp. Licensed under LGPL v2. @see https://github.com/naver/smarteditor2/blob/master/LICENSE.md */ ! function(o) {
    var t = {};

    function i(e) {
        if (t[e]) return t[e].exports;
        var n = t[e] = {
            i: e,
            l: !1,
            exports: {}
        };
        return o[e].call(n.exports, n, n.exports, i), n.l = !0, n.exports
    }
    i.m = o, i.c = t, i.d = function(e, n, o) {
        i.o(e, n) || Object.defineProperty(e, n, {
            enumerable: !0,
            get: o
        })
    }, i.r = function(e) {
        "undefined" != typeof Symbol && Symbol.toStringTag && Object.defineProperty(e, Symbol.toStringTag, {
            value: "Module"
        }), Object.defineProperty(e, "__esModule", {
            value: !0
        })
    }, i.t = function(n, e) {
        if (1 & e && (n = i(n)), 8 & e) return n;
        if (4 & e && "object" == typeof n && n && n.__esModule) return n;
        var o = Object.create(null);
        if (i.r(o), Object.defineProperty(o, "default", {
                enumerable: !0,
                value: n
            }), 2 & e && "string" != typeof n)
            for (var t in n) i.d(o, t, function(e) {
                return n[e]
            }.bind(null, t));
        return o
    }, i.n = function(e) {
        var n = e && e.__esModule ? function() {
            return e["default"]
        } : function() {
            return e
        };
        return i.d(n, "a", n), n
    }, i.o = function(e, n) {
        return Object.prototype.hasOwnProperty.call(e, n)
    }, i.p = "", i(i.s = 47)
}({
    47: function(e, n, o) {
        "use strict";
        o(48), o(49), o(50), o(51), o(52), o(53), o(54), o(55)
    },
    48: function(e, n) {
        nhn.husky.SE2B_CSSLoader = jindo.$Class({
            name: "SE2B_CSSLoader",
            bCssLoaded: !1,
            aInstantLoadTrigger: ["OPEN_QE_LAYER", "SHOW_ACTIVE_LAYER", "SHOW_DIALOG_LAYER", "START_SPELLCHECK"],
            aDelayedLoadTrigger: ["MSG_SE_OBJECT_EDIT_REQUESTED", "OBJECT_MODIFY", "MSG_SE_DUMMY_OBJECT_EDIT_REQUESTED", "TOGGLE_TOOLBAR_ACTIVE_LAYER", "SHOW_TOOLBAR_ACTIVE_LAYER"],
            $init: function() {
                if (this.htOptions = nhn.husky.SE2M_Configuration.SE2B_CSSLoader, jindo.$Agent().navigator().ie) {
                    for (var e = 0, n = this.aInstantLoadTrigger.length; e < n; e++) this["$BEFORE_" + this.aInstantLoadTrigger[e]] = jindo.$Fn(function() {
                        this.loadSE2CSS()
                    }, this).bind();
                    for (e = 0, n = this.aDelayedLoadTrigger.length; e < n; e++) {
                        var o = this.aDelayedLoadTrigger[e];
                        this["$BEFORE_" + this.aDelayedLoadTrigger[e]] = jindo.$Fn(function(e) {
                            var n = jindo.$A(arguments).$value();
                            return n = n.splice(1, n.length - 1), this.loadSE2CSS(e, n)
                        }, this).bind(o)
                    }
                } else this.$ON_MSG_APP_READY = jindo.$Fn(function() {
                    this.loadSE2CSS()
                }, this).bind()
            },
            loadSE2CSS: function(e, n) {
                if (this.bCssLoaded) return !0;
                this.bCssLoaded = !0;
                var o = null;
                e && (o = jindo.$Fn(this.oApp.exec, this.oApp).bind(e, n));
                var t = this.htOptions.sCSSBaseURI,
                    i = this.oApp && this.oApp.htOptions.I18N_LOCALE;
                return i && (t += "/" + i), t += "/smart_editor2_items.css", nhn.husky.SE2M_Utils.loadCSS(t, o), !1
            }
        })
    },
    49: function(e, n) {
        "undefined" == typeof window.nhn && (window.nhn = {}), window.oMessageMap = {
            "SE_EditingAreaManager.onExit": "내용이 변경되었습니다.",
            "SE_Color.invalidColorCode": "색상 코드를 올바르게 입력해 주세요. \n\n 예) #000000, #FF0000, #FFFFFF, #ffffff, ffffff",
            "SE_Hyperlink.invalidURL": "입력하신 URL이 올바르지 않습니다.",
            "SE_FindReplace.keywordMissing": "찾으실 단어를 입력해 주세요.",
            "SE_FindReplace.keywordNotFound": "찾으실 단어가 없습니다.",
            "SE_FindReplace.replaceAllResultP1": "일치하는 내용이 총 ",
            "SE_FindReplace.replaceAllResultP2": "건 바뀌었습니다.",
            "SE_FindReplace.notSupportedBrowser": "현재 사용하고 계신 브라우저에서는 사용하실수 없는 기능입니다.\n\n이용에 불편을 드려 죄송합니다.",
            "SE_FindReplace.replaceKeywordNotFound": "바뀔 단어가 없습니다",
            "SE_LineHeight.invalidLineHeight": "잘못된 값입니다.",
            "SE_Footnote.defaultText": "각주내용을 입력해 주세요",
            "SE.failedToLoadFlash": "플래시가 차단되어 있어 해당 기능을 사용할 수 없습니다.",
            "SE2M_EditingModeChanger.confirmTextMode": "텍스트 모드로 전환하면 작성된 내용은 유지되나, \n\n글꼴 등의 편집효과와 이미지 등의 첨부내용이 모두 사라지게 됩니다.\n\n전환하시겠습니까?",
            "SE2M_FontNameWithLayerUI.sSampleText": "가나다라"
        }
    },
    50: function(e, n) {
        "undefined" == typeof window.nhn && (window.nhn = {}), window.oMessageMap_en_US = {
            "SE_EditingAreaManager.onExit": "Contents have been changed.",
            "SE_Color.invalidColorCode": "Enter the correct color code. \n\n ex) #000000, #FF0000, #FFFFFF, #ffffff, ffffff",
            "SE_Hyperlink.invalidURL": "You have entered an incorrect URL.",
            "SE_FindReplace.keywordMissing": "Enter the word you wish to find.",
            "SE_FindReplace.keywordNotFound": "The word does not exist.",
            "SE_FindReplace.replaceAllResultP1": "A total of ",
            "SE_FindReplace.replaceAllResultP2": " matching contents have been changed.",
            "SE_FindReplace.notSupportedBrowser": "Function cannot be used in the browser you are currently using. \n\nSorry for the inconvenience.",
            "SE_FindReplace.replaceKeywordNotFound": "No word to change.",
            "SE_LineHeight.invalidLineHeight": "Incorrect value.",
            "SE_Footnote.defaultText": "Enter footnote details.",
            "SE.failedToLoadFlash": "The function cannot be used because flash has been blocked.",
            "SE2M_EditingModeChanger.confirmTextMode": "The contents remain, but editing effects, including fonts, and attachments, \n\nsuch as images, will disappear when changed to text mode. \n\n Make changes?",
            "SE2M_FontNameWithLayerUI.sSampleText": "ABCD"
        }
    },
    51: function(e, n) {
        "undefined" == typeof window.nhn && (window.nhn = {}), window.oMessageMap_ja_JP = {
            "SE_EditingAreaManager.onExit": "内容が変更されました。",
            "SE_Color.invalidColorCode": "カラーコードを正しく入力してください。 \n\n 例) #000000, #FF0000, #FFFFFF, #ffffff, ffffff",
            "SE_Hyperlink.invalidURL": "入力したURLが正しくありません。",
            "SE_FindReplace.keywordMissing": "お探しの単語を入力してください。",
            "SE_FindReplace.keywordNotFound": "お探しの単語がありません。",
            "SE_FindReplace.replaceAllResultP1": "一致する内容が計",
            "SE_FindReplace.replaceAllResultP2": "件変わりました。",
            "SE_FindReplace.notSupportedBrowser": "現在ご使用中のブラウザーではご利用いただけない機能です。\n\nご不便をおかけしまして申し訳ございません。",
            "SE_FindReplace.replaceKeywordNotFound": "変更される単語がありません。",
            "SE_LineHeight.invalidLineHeight": "誤った値です。",
            "SE_Footnote.defaultText": "脚注内容を入力してください。",
            "SE.failedToLoadFlash": "フラッシュが遮断されているため、この機能は使用できません。",
            "SE2M_EditingModeChanger.confirmTextMode": "テキストモードに切り換えると、作成された内容は維持されますが、\n\nフォント等の編集効果と画像等の添付内容が消えることになります。\n\n切り換えますか？",
            "SE2M_FontNameWithLayerUI.sSampleText": "あいうえお"
        }
    },
    52: function(e, n) {
        "undefined" == typeof window.nhn && (window.nhn = {}), window.oMessageMap_zh_CN = {
            "SE_EditingAreaManager.onExit": "内容有了变化。",
            "SE_Color.invalidColorCode": "请你输入正确的色相代码。 \n\n 例) #000000, #FF0000, #FFFFFF, #ffffff, ffffff",
            "SE_Hyperlink.invalidURL": "你输入的URL不符条件。",
            "SE_FindReplace.keywordMissing": "请你输入要找的词汇。",
            "SE_FindReplace.keywordNotFound": "没有词汇符合条件。",
            "SE_FindReplace.replaceAllResultP1": "符合条件的内容改编为",
            "SE_FindReplace.replaceAllResultP2": "件",
            "SE_FindReplace.notSupportedBrowser": "这是你现在使用的浏览器不可支持的功能。\n\n麻烦你很道歉。",
            "SE_FindReplace.replaceKeywordNotFound": "没有词汇要改变。",
            "SE_LineHeight.invalidLineHeight": "这是有问题的值。",
            "SE_Footnote.defaultText": "请你输入脚注内容。",
            "SE.failedToLoadFlash": "flash被隔绝，不能使用该功能。",
            "SE2M_EditingModeChanger.confirmTextMode": "转换为text模式就能维持制作内容，\n\n但字体等编辑效果和图像等附件内容都会消失。\n\n你还要继续吗？",
            "SE2M_FontNameWithLayerUI.sSampleText": "ABCD"
        }
    },
    53: function(e, n) {
        "undefined" == typeof window.nhn && (window.nhn = {}), window.oMessageMap_zh_TW = {
            "SE_EditingAreaManager.onExit": "內容已被更改。",
            "SE_Color.invalidColorCode": "請輸入正確的顔色代碼。\n\n例子）#000000, #FF0000, #FFFFFF, #ffffff, ffffff",
            "SE_Hyperlink.invalidURL": "輸錯URL",
            "SE_FindReplace.keywordMissing": "請輸入要查詢的單詞。",
            "SE_FindReplace.keywordNotFound": "要查詢的單詞不存在。",
            "SE_FindReplace.replaceAllResultP1": "一致的內容總有",
            "SE_FindReplace.replaceAllResultP2": "件已被更改。",
            "SE_FindReplace.notSupportedBrowser": "當前的浏覽器上無法使用此功能。\n\n很抱歉給你添麻煩了。",
            "SE_FindReplace.replaceKeywordNotFound": "要更改的詞彙不存在。",
            "SE_LineHeight.invalidLineHeight": "爲有錯誤的值。",
            "SE_Footnote.defaultText": "請輸入注腳內容。",
            "SE.failedToLoadFlash": "Flash被屏蔽，導致無法使用此功能。",
            "SE2M_EditingModeChanger.confirmTextMode": "如轉換爲文本模式，雖然維持已寫成的內容，\n\n但字號等編輯效果和圖像等附加內容都會被消失。\n\n是否轉換？",
            "SE2M_FontNameWithLayerUI.sSampleText": "ABCD"
        }
    },
    54: function(e, n) {
        nhn.husky.SE_OuterIFrameControl = jindo.$Class({
            name: "SE_OuterIFrameControl",
            oResizeGrip: null,
            $init: function(e) {
                this.aHeightChangeKeyMap = [-100, 100, 500, -500, -1, -10, 1, 10], this._assignHTMLObjects(e), this.$FnKeyDown = jindo.$Fn(this._keydown, this), this.oResizeGrip && this.$FnKeyDown.attach(this.oResizeGrip, "keydown"), jindo.$Agent().navigator().ie && (this.$FnMouseDown = jindo.$Fn(this._mousedown, this), this.$FnMouseMove = jindo.$Fn(this._mousemove, this), this.$FnMouseMove_Parent = jindo.$Fn(this._mousemove_parent, this), this.$FnMouseUp = jindo.$Fn(this._mouseup, this), this.oResizeGrip && this.$FnMouseDown.attach(this.oResizeGrip, "mousedown"))
            },
            _assignHTMLObjects: function(e) {
                e = jindo.$(e) || document, this.oResizeGrip = jindo.cssquery.getSingle(".husky_seditor_editingArea_verticalResizer", e), this.elIFrame = window.frameElement, this.welIFrame = jindo.$Element(this.elIFrame)
            },
            $ON_MSG_APP_READY: function() {
                this.oApp.exec("SE_FIT_IFRAME", [])
            },
            $ON_MSG_EDITING_AREA_SIZE_CHANGED: function() {
                this.oApp.exec("SE_FIT_IFRAME", [])
            },
            $ON_SE_FIT_IFRAME: function() {
                this.elIFrame.style.height = document.body.offsetHeight + "px"
            },
            $AFTER_RESIZE_EDITING_AREA_BY: function() {
                this.oApp.exec("SE_FIT_IFRAME", [])
            },
            _keydown: function(e) {
                var n = e.key();
                33 <= n.keyCode && n.keyCode <= 40 && (this.oApp.exec("MSG_EDITING_AREA_RESIZE_STARTED", []), this.oApp.exec("RESIZE_EDITING_AREA_BY", [0, this.aHeightChangeKeyMap[n.keyCode - 33]]), this.oApp.exec("MSG_EDITING_AREA_RESIZE_ENDED", []), e.stop())
            },
            _mousedown: function(e) {
                this.iStartHeight = e.pos().clientY, this.iStartHeightOffset = e.pos().layerY, this.$FnMouseMove.attach(document, "mousemove"), this.$FnMouseMove_Parent.attach(parent.document, "mousemove"), this.$FnMouseUp.attach(document, "mouseup"), this.$FnMouseUp.attach(parent.document, "mouseup"), this.iStartHeight = e.pos().clientY, this.oApp.exec("MSG_EDITING_AREA_RESIZE_STARTED", [this.$FnMouseDown, this.$FnMouseMove, this.$FnMouseUp])
            },
            _mousemove: function(e) {
                var n = e.pos().clientY - this.iStartHeight;
                this.oApp.exec("RESIZE_EDITING_AREA_BY", [0, n])
            },
            _mousemove_parent: function(e) {
                var n = e.pos().pageY - (this.welIFrame.offset().top + this.iStartHeight);
                this.oApp.exec("RESIZE_EDITING_AREA_BY", [0, n])
            },
            _mouseup: function() {
                this.$FnMouseMove.detach(document, "mousemove"), this.$FnMouseMove_Parent.detach(parent.document, "mousemove"), this.$FnMouseUp.detach(document, "mouseup"), this.$FnMouseUp.detach(parent.document, "mouseup"), this.oApp.exec("MSG_EDITING_AREA_RESIZE_ENDED", [this.$FnMouseDown, this.$FnMouseMove, this.$FnMouseUp])
            }
        })
    },
    55: function(e, n) {
        nhn.husky.SE_ToolbarToggler = jindo.$Class({
            name: "SE_ToolbarToggler",
            bUseToolbar: !0,
            $init: function(e, n) {
                this._assignHTMLObjects(e, n)
            },
            _assignHTMLObjects: function(e, n) {
                e = jindo.$(e) || document, this.toolbarArea = jindo.cssquery.getSingle(".se2_tool", e), this.toolbarArea.style.display = void 0 === n || !0 === n ? "block" : "none"
            },
            $ON_MSG_APP_READY: function() {
                this.oApp.exec("REGISTER_HOTKEY", ["ctrl+t", "SE_TOGGLE_TOOLBAR", []])
            },
            $ON_SE_TOGGLE_TOOLBAR: function() {
                this.toolbarArea.style.display = "none" == this.toolbarArea.style.display ? "block" : "none", this.oApp.exec("MSG_EDITING_AREA_SIZE_CHANGED", [])
            }
        })
    }
});