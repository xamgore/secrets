NodeList.prototype.forEach = Array.prototype.forEach;
$ = selector => document.querySelector(selector);

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
        if ($('.error') !== null)
            $('.error').select();
        else focus();
    }}, 1);

// prevent form submit when fields are empty
$('form').onsubmit = e => {
    if (!login.value || !pass.value) { focus(); e.preventDefault(); }};

var showCursor = (node) => {
    node.className = (node.selectionEnd == node.value.length ? "nocursor" : "");
    if (node.hasErrorBorder) node.className += " error";
};

var showCursorFor = node => { return () => showCursor(node); };

// remove red border on edit
var noerror = node => {
    return e => {
        if (node.pval != node.value) {
            node.hasErrorBorder = false; node.pval = node.value;
        }
        showCursor(node);
    };
};

[login, pass].forEach(x => {
    x.pval = x.value;
    x.onkeyup = noerror(x);
    x.onclick = x.onfocus = showCursorFor(x);
});

// select invalid field
$('.error').select().hasErrorBorder = true;
