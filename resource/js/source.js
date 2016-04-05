NodeList.prototype.forEach = Array.prototype.forEach;
$ = (selector) => document.querySelector(selector);

var login = $('#login'), pass = $('#pass'), submit = $('#submit');

// intelli focus technique
var focus = () => {
    if (!login.value) login.focus();
    else if (!pass.value) pass.focus();
    else submit.focus();
};

// [login, pass, submit].forEach(x => x.onkeypress = focus);

// set focus if it was lost
window.setInterval(() => {
    if ([login, pass, submit].indexOf(document.activeElement) == -1) {
        if ($('.nocursor.error') !== null)
            $('.nocursor.error').select();
        else focus();
    }}, 1);

// prevent form submit when fields are empty
$('form').onsubmit = e => {
    if (!login.value || !pass.value) { focus(); e.preventDefault(); }};

// remove red border on edit
var noerror = (node) => {
    return (e) => {
        if (node.pval != node.value) {
            node.className = "nocursor";
            x.pval = x.value;
        }
    };
};
[login, pass].forEach(x => { x.pval = x.value; x.onkeyup = noerror(x); });

// select invalid field
$('.nocursor.error').select();
