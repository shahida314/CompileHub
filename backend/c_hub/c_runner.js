const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

function runCCode(code, callback) {
    const tempPath = path.join(__dirname, 'temp_code.txt');
    fs.writeFileSync(tempPath, code);

    const compilerExe = path.join(__dirname, 'compiler.exe');

    exec(`"${compilerExe}" "${tempPath}"`, (error, stdout, stderr) => {
        if (fs.existsSync(tempPath)) fs.unlinkSync(tempPath);

        if (error || stderr) {
            callback({ error: stderr || error.message });
        } else {
            callback({ output: stdout });
        }
    });
}

module.exports = { runCCode };