#ifndef __HTML_FREGMENTS_H__
#define __HTML_FREGMENTS_H__

namespace cooboc {

/* clang-format off */
static const char htmlHeader1[] PROGMEM = {
"<!doctype html><html lang='cn'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Cooboc Smart Setup</title><meta name='description' content='Cooboc Smart Device Setup'><meta name='author' content='HE Qichen'>"
};

static const char htmlHeader2[] PROGMEM = {
"<meta http-equiv='refresh' content='5; URL=/'>"
};

static const char htmlHeader3[] PROGMEM = {
"<style>*{margin:2px}</style></head><body><h1>Cooboc Smart Device Setup</h1>"
};

static const char htmlSetupBody1[] PROGMEM = {
"ID:<b>CoobocSmart_"
};

static const char htmlSetupBody2[] PROGMEM = {
"</b><br><form action='/set' method='POST' id='form'><label for='s'>SSID:</label><input name='s' type='text'><br><label for='w'>Password:</label><input name='w' type='text'><br><label for='h'>Host: </label><input name='h' type='text' maxlength='15'><br><label for='c'>Config:</label><input name='c' type='text'><br><input id='p' type='submit' value='Submit'></form><script lang='text/javascript'>var n,s=['None'"
};

static const char htmlSetupBody3[] PROGMEM = {
"];d=document,f=(n,e,t)=>{n.setAttribute(e,t)},g=n=>d.createElement(n),p=d.getElementById('p'),h=n=>{p.parentNode.insertBefore(n,p)};for(var i=0;i<s.length;++i)n=g('input'),f(n,'type','radio'),f(n,'id',i),f(n,'name','t'),f(n,'value',i-1),h(n),n=g('label'),f(n,'for',i),n.innerText=s[i],h(n),h(g('br'));</script></body></html>"
};
/* clang-format on */

} // namespace cooboc

#endif
