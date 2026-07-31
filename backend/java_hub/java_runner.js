const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

function runJavaCompiler(code, callback) {
    const tempFilePath = path.join(__dirname, 'temp_input.java');
    
    
    fs.writeFileSync(tempFilePath, code);

    const executablePath = path.join(__dirname, 'java_compiler.exe');
    exec(`"${executablePath}" "${tempFilePath}"`, (error, stdout, stderr) => {
      
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