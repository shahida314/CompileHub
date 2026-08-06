const { spawn } = require('child_process');
const fs = require('fs');
const path = require('path');

function runCppCode(code, input, callback) {
  const tempPath = path.join(__dirname, 'temp_cpp_code.txt');
  
  
  fs.writeFileSync(tempPath, code);

  const compilerExe = path.join(__dirname, 'cpp_compiler.exe');

  // Compiler Process Spawn
  const child = spawn(compilerExe, [tempPath], { cwd: __dirname });

  let output = '';
  let errorOutput = '';
  let finished = false;

  function cleanup() {
    if (fs.existsSync(tempPath)) {
      try { fs.unlinkSync(tempPath); } catch (e) {}
    }
  }

  const timer = setTimeout(() => {
    if (!finished) {
      finished = true;
      child.kill();
      cleanup();
      callback({ error: 'Execution timed out.' });
    }
  }, 5000);

  child.stdout.on('data', (d) => { output += d.toString(); });
  child.stderr.on('data', (d) => { errorOutput += d.toString(); });

  if (input && input.trim() !== '') {
    child.stdin.write(input.endsWith('\n') ? input : input + '\n');
  }
  child.stdin.end();

  child.on('close', (code) => {
    if (finished) return;
    finished = true;
    clearTimeout(timer);
    cleanup();

    if (errorOutput) {
      callback({ error: errorOutput });
    } else {
      callback({ output: output || '(No output produced)' });
    }
  });

  child.on('error', (err) => {
    if (finished) return;
    finished = true;
    clearTimeout(timer);
    cleanup();
    callback({ error: 'Failed to run compiler: ' + err.message });
  });
}

module.exports = { runCppCode };