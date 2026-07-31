const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

function runJavaCompiler(code, callback) {
    const tempFilePath = path.join(__dirname, 'temp_input.java');
    
    // ১. ফ্রন্টএন্ডের জাভা কোড টেম্পোরারি ফাইলে সেভ করা
    fs.writeFileSync(tempFilePath, code);

    // ২. জেনারেটেড exe দিয়ে কোড টেস্ট করা
    const executablePath = path.join(__dirname, 'java_compiler.exe');
    exec(`"${executablePath}" "${tempFilePath}"`, (error, stdout, stderr) => {
        // টেম্পোরারি ফাইল মুছে ফেলা
        if (fs.existsSync(tempFilePath)) {
            fs.unlinkSync(tempFilePath);
        }

        if (error) {
            callback(stderr || error.message);
            return;
        }
        callback(stdout);
    });
}

module.exports = { runJavaCompiler };