#include "StdAfx.h"

CString g_html_template = R"(
<STYLE TYPE = "text/css"><!--
.content{
    border - right - width: 0px;
    background - color: white;
    font - size: 12px;
    font - family: Consolas, Courier New, Arial;
    text - align: left;
    white - space: -moz - pre - wrap; /* Mozilla based browsers */
    white - space: -pre - wrap;     /* Opera 4 - 6 */
    white - space: -o - pre - wrap;   /* Opera >= 7 */
    white - space: pre - wrap;      /* CSS3 */
    word - wrap: break - word;      /* IE 5.5+ */
}

table.reference
{
    background - color:#ffffff;
border:1px solid #c3c3c3;
    border - collapse:collapse;
    font - family:Consolas, Courier New, Arial;
    font - size:12px;
}

table.reference th
{
    background - color:#e5eecc;
border:1px solid #c3c3c3;
padding:3px;
    vertical - align:top;
}

table.reference td
{
border:1px solid #c3c3c3;
padding:3px;
    vertical - align:top;
    text - align:center;
}

h2.result, h2.result_head
{
color:#617f10;
    background - color:transparent;
    margin - top:0px;
}

h2.result{ font - size:125 % ; }
h2.result_head{ font - size:140 % ; }

.page
{
    font - family:Consolas, Courier New, Arial;
    font - size:12px;
}
.button1
{
    font - family:Consolas, Arial, Courier New;
    font - size:12px;
width:100 % ;
height:30px;
    border - width:0px;
    background - color:#E0E0E0;
}
.button2
{
    font - family:Consolas, Arial, Courier New;
    font - size:12px;
width:100 % ;
height:30px;
    border - width:0px;
    background - color:#CCCCCC;
}


.detail
{
    /*	visibility: hidden;*/
}
-->
</STYLE>

)";
