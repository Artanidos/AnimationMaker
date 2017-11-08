window.onload = function(){
var tween1_0 = TweenLite.to("#ARTANANDA1", 0, {attr:{x:202}, delay: 0});
var tween1_0 = TweenLite.to("#ARTANANDA1", 0, {attr:{y:311}, delay: 0});
var tween1_1 = TweenLite.to("#ARTANANDA1", 0.5, {attr:{y:274}, delay: 9, ease:Linear.easeNone});
var tween2_0 = TweenLite.to("#Logo2", 0, {attr:{y:314}, delay: 0});
var tween2_1 = TweenLite.to("#Logo2", 0.5, {attr:{y:244}, delay: 9, ease:Linear.easeNone});
var tween3_0 = TweenLite.to("#Es_ist_unmöglich_3", 0, {attr:{x:421}, delay: 0});
var tween3_1 = TweenLite.to("#Es_ist_unmöglich_3", 1, {attr:{x:70}, delay: 0, ease:Linear.easeNone});
var tween5_0 = TweenLite.to("#Es_ist_riskant_5", 0, {attr:{x:421}, delay: 0});
var tween5_1 = TweenLite.to("#Es_ist_riskant_5", 1, {attr:{x:70}, delay: 2, ease:Linear.easeNone});
var tween7_0 = TweenLite.to("#Es_ist_sinnlos_7", 0, {attr:{x:416}, delay: 0});
var tween7_1 = TweenLite.to("#Es_ist_sinnlos_7", 1, {attr:{x:70}, delay: 4, ease:Linear.easeNone});
var tween9_0 = TweenLite.to("#VERSUCH_ES_9", 0, {attr:{x:417}, delay: 0});
var tween9_1 = TweenLite.to("#VERSUCH_ES_9", 1, {attr:{x:70}, delay: 6, ease:Linear.easeNone});
var tween11_0 = TweenLite.to("#sagt_der_Stolz11", 0, {attr:{x:420}, delay: 0});
var tween11_1 = TweenLite.to("#sagt_der_Stolz11", 0.5, {attr:{x:70}, delay: 1, ease:Linear.easeNone});
var tween13_0 = TweenLite.to("#flüstert_das_HERZ13", 0, {attr:{x:415}, delay: 0});
var tween13_1 = TweenLite.to("#flüstert_das_HERZ13", 0.5, {attr:{x:70}, delay: 7, ease:Linear.easeNone});
var tween15_0 = TweenLite.to("#sagt_die_Erfahrung15", 0, {attr:{x:423}, delay: 0});
var tween15_1 = TweenLite.to("#sagt_die_Erfahrung15", 0.5, {attr:{x:70}, delay: 3, ease:Linear.easeNone});
var tween17_0 = TweenLite.to("#sagt_der_Verstand17", 0, {attr:{x:418}, delay: 0});
var tween17_1 = TweenLite.to("#sagt_der_Verstand17", 0.5, {attr:{x:70}, delay: 5, ease:Linear.easeNone});
var tl = new TimelineLite();
tl.add([tween1_0, tween1_0, tween1_1, tween2_0, tween2_1, tween3_0, tween3_1, tween5_0, tween5_1, tween7_0, tween7_1, tween9_0, tween9_1, tween11_0, tween11_1, tween13_0, tween13_1, tween15_0, tween15_1, tween17_0, tween17_1]);
tl.play();
}
