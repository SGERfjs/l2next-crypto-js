const L2NextCryptoJS = require('bindings')('L2NextCryptoJS');
var fs = require('fs');
 
const file = 'C:/Projects/playground/system-hf-p5/armorgrp.dat';

fs.readFile(file, (err, contents) => {
    if (err) return console.log(`Error reading ${file}`);

    L2NextCryptoJS.decrypt(contents.toString(), contents.byteLength, (dec) => {
        console.log('Result:', dec)
   
    });
});