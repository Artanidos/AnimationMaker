/**	STYLE SWITCHER
*************************************************** **/
jQuery(document).ready(function() {
	"use strict";

    jQuery("#hideSwitcher, #showSwitcher").click(function () {

        if (jQuery("#showSwitcher").is(":visible")) {

			var _identifier = "#showSwitcher";
            jQuery("#switcher").animate({"margin-left": "0px"}, 500).show();
			createCookie("switcher_visible", 'true', 365);

        } else {

			var _identifier = "#switcher";
            jQuery("#showSwitcher").show().animate({"margin-left": "0"}, 500);
			createCookie("switcher_visible", 'false', 365);

        }

		jQuery(_identifier).animate({"margin-left": "-500px"}, 500, function () {
			jQuery(this).hide();
		});

    });
                      
});

function setActiveStyleSheet(title) {
	var i, a, main;
	for(i=0; (a = document.getElementsByTagName("link")[i]); i++) {
		if(a.getAttribute("rel").indexOf("style") != -1 && a.getAttribute("title")) {
			a.disabled = true;
			if(a.getAttribute("title") == title) { a.disabled = false; }
		}
	}

	// DARK SKIN
	var color_skin = readCookie('color_skin');
	if(color_skin == 'dark') {
		jQuery("#css_dark_skin").remove();
		jQuery("head").append('<link id="css_dark_skin" href="assets/css/layout-dark.css" rel="stylesheet" type="text/css" title="dark" />');
		jQuery("#is_dark").trigger('click');
		jQuery("a.logo img").attr('src', 'assets/images/logo_dark.png');
	}
}

function getActiveStyleSheet() {
	var i, a;
	for(i=0; (a = document.getElementsByTagName("link")[i]); i++) {
		if(a.getAttribute("rel").indexOf("style") != -1 && a.getAttribute("title") && !a.disabled) { return a.getAttribute("title"); }
	}

	return null;
}

function getPreferredStyleSheet() {
	var i, a;
	for(i=0; (a = document.getElementsByTagName("link")[i]); i++) {
		if(a.getAttribute("rel").indexOf("style") != -1 && a.getAttribute("rel").indexOf("alt") == -1 && a.getAttribute("title")) { 
			return a.getAttribute("title"); 
		}
	}

	return null;
}

function createCookie(name,value,days) {
	if (days) {
		var date = new Date();
		date.setTime(date.getTime()+(days*24*60*60*1000));
		var expires = "; expires="+date.toGMTString();
	} else {
		expires = "";
	}	document.cookie = name+"="+value+expires+"; path=/";
}

function readCookie(name) {
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];

		while (c.charAt(0)==' ') {
			c = c.substring(1,c.length);
		}

		if (c.indexOf(nameEQ) == 0) {
			return c.substring(nameEQ.length,c.length);
		}
	}

	return null;
}


/** ********************************************************************************************************** **/
/** ********************************************************************************************************** **/
/** ********************************************************************************************************** **/

/**
	@ON LOAD
**/
window.onload = function(e) {

	// COLOR SCHEME
	var cookie = readCookie("style");
	var title = cookie ? cookie : getPreferredStyleSheet();
	setActiveStyleSheet(title);

	// SWITCHER OPEN|CLOSED
	var switcher_visible = readCookie('switcher_visible');
	if(switcher_visible != 'false') {
		jQuery("#showSwitcher").trigger('click');
	}

	// DARK SKIN
	var color_skin = readCookie('color_skin');
	if(color_skin == 'dark') {
		jQuery("#css_dark_skin").remove();
		jQuery("head").append('<link id="css_dark_skin" href="assets/css/layout-dark.css" rel="stylesheet" type="text/css" title="dark" />');
		jQuery("#is_dark").trigger('click');
		jQuery("a.logo img").attr('src', 'assets/images/logo_dark.png');
	}

	// BOXED or WIDE
	var is_boxed = readCookie('is_boxed');
	if(is_boxed == 'true') {
		jQuery('body').removeClass('boxed');
		jQuery('body').addClass('boxed');
		jQuery("#is_boxed").trigger('click');
	}

	// PATTERN BACKGROUND
	var pattern_switch = readCookie('pattern_switch');
	if(pattern_switch) {
			// switch to boxed first!
			jQuery('body').removeClass('boxed');
			jQuery('body').addClass('boxed');
			jQuery("#is_boxed").trigger('click');

			remove_pattern();
			jQuery('body').addClass(pattern_switch);
	}

	// BACKGROUND IMAGE
	var data_background = readCookie('background_switch');
	if(data_background) {
			jQuery.backstretch(data_background);
			jQuery('body').addClass('transparent'); // remove backround color of boxed class
	}
	
}

window.onunload = function(e) {
	var title = getActiveStyleSheet();
	createCookie("style", title, 365);
}

var cookie 	= readCookie("style");
var title 	= cookie ? cookie : getPreferredStyleSheet();
setActiveStyleSheet(title);

jQuery(document).ready(function() {

	jQuery("select#headerLayout").click(function() {

		var type = jQuery(this).attr('value');

		if (jQuery("#css_navigation_style").length > 0){
			jQuery("#css_navigation_style").remove();
		}

		jQuery("head").append("<link>");
		jQuery("head").children(":last").attr({
			rel:  	"stylesheet",
			type: 	"text/css",
			id: 	"css_navigation_style",
			href: 	"css/navigation-style-" + type + ".css"
		});

	});

}); 


/**
	COLOR SKIN [light|dark]
**/
jQuery("input.dark_switch").bind("click", function() {
	var color_skin = jQuery(this).attr('value');

	if(color_skin == 'dark') {
		jQuery("#css_dark_skin").remove();
		jQuery("head").append('<link id="css_dark_skin" href="assets/css/layout-dark.css" rel="stylesheet" type="text/css" title="dark" />');
		createCookie("color_skin", 'dark', 365);
		jQuery("a.logo img").attr('src', 'assets/images/logo_dark.png');
	} else {
		jQuery("#css_dark_skin").remove();
		createCookie("color_skin", '', -1);
		jQuery("a.logo img").attr('src', 'assets/images/logo.png');
	}
});





/**
	LAYOUT STYLE [wide|boxed]
**/
jQuery("input.boxed_switch").bind("click", function() {
	var boxed_switch = jQuery(this).attr('value');

	if(boxed_switch == 'boxed') {
		jQuery("body").removeClass('boxed');
		jQuery("body").addClass('boxed');
		createCookie("is_boxed", 'true', 365);
	} else {
		jQuery("body").removeClass('boxed');
		createCookie("is_boxed", '', -1);
		jQuery('body').removeClass('transparent');
	}
});




/**
	Pattern Background
**/
function pattern_switch(pattern) {
	if(pattern == 'none' || pattern == '') {
		createCookie("pattern_switch", pattern, -1);
		remove_pattern();
	} else {

		if(!jQuery('body').hasClass('boxed')) {
			jQuery('body').addClass('boxed');
			jQuery("#is_boxed").trigger('click');
			createCookie("is_boxed", 'true', 365);
		}

		createCookie("background_switch", '', -1);
		jQuery('body').attr('data-background', '');
		jQuery('.backstretch').remove();
		jQuery('body').removeClass('transparent');
		remove_pattern();

		remove_pattern();
		jQuery('body').addClass(pattern);
		createCookie("pattern_switch", pattern, 365);
	}
}
function remove_pattern() {
	jQuery('body').removeClass('pattern1');
	jQuery('body').removeClass('pattern2');
	jQuery('body').removeClass('pattern3');
	jQuery('body').removeClass('pattern4');
	jQuery('body').removeClass('pattern5');
	jQuery('body').removeClass('pattern6');
	jQuery('body').removeClass('pattern7');
	jQuery('body').removeClass('pattern8');
	jQuery('body').removeClass('pattern9');
	jQuery('body').removeClass('pattern10');
	createCookie("pattern_switch", '', -1);
}



/**
	Image Background
**/
function background_switch(imgbkg) {
	if(imgbkg == 'none' || imgbkg == '') {

		createCookie("background_switch", '', -1);
		jQuery('body').attr('data-background', '');
		jQuery('.backstretch').remove();
		jQuery('body').removeClass('transparent');
		remove_pattern();

	} else {

		if(!jQuery('body').hasClass('boxed')) {
			jQuery('body').addClass('boxed');
			jQuery("#is_boxed").trigger('click');
			createCookie("is_boxed", 'true', 365);
		}

		jQuery('body').attr('data-background', imgbkg);
		createCookie("background_switch", imgbkg, 365);
		remove_pattern();

		var data_background = jQuery('body').attr('data-background');
		if(data_background) {
			jQuery.backstretch(data_background);
			jQuery('body').addClass('transparent'); // remove backround color of boxed class
		}
	}
}



/**
	Reset Switcher
**/
function resetSwitcher() {
	remove_pattern();
	jQuery('body').removeClass('boxed');
	jQuery("#css_dark_skin").remove();
	jQuery('body').attr('data-background', '');
	jQuery('.backstretch').remove();
	jQuery("a.logo img").attr('src', 'assets/images/logo.png');

	jQuery("#is_light").trigger('click');
	jQuery("#is_wide").trigger('click');

	// delete cookies!
	createCookie("style", '', -1);
	createCookie("switcher_visible", '', -1);
	createCookie("pattern_switch", '', -1);
	createCookie("background_switch", '', -1);
	createCookie("boxed", '', -1);
	createCookie("color_skin", '', -1);
	createCookie("is_boxed", '', -1);
}