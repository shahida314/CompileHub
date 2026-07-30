const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

function runJavaCompiler(codeFilePath) {
    return new Promise((resolve, reject) => {
        const compilerPath = path.join(__dirname, 'java_compiler.exe');
        
        exec(`"${compilerPath}" "${codeFilePath}"`, (error, stdout, stderr) => {
            if (error) {
                resolve({
                    success: false,
                    output: stdout,
                    error: stderr || error.message
                });
            } else {
                resolve({
                    success: true,
                    output: stdout,
                    error: stderr
                });
            }
        });
    });
}

module.exports = { runJavaCompiler };