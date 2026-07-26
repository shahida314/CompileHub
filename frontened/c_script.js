// Default Starter Code for C
const defaultCCode = `#include <stdio.h>\n\nint main() {\n    printf("Hello World\\n");\n    return 0;\n}`;

document.addEventListener('DOMContentLoaded', () => {
    const languageSelect = document.getElementById('languageSelect');
    const codeArea = document.getElementById('codeArea');
    const lineNumbers = document.getElementById('lineNumbers');
    const runBtn = document.getElementById('runBtn');
    const consoleOutput = document.getElementById('consoleOutput');

    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');

    // Render Backend API Endpoint for C
    const C_BACKEND_URL = 'http://localhost:5000/api/compile/c';

    // Default Initialization
    codeArea.value = defaultCCode;
    updateLineNumbers();

    // Line Number Update Logic
    function updateLineNumbers() {
        const lines = codeArea.value.split('\n').length;
        let lineHTML = '';
        for (let i = 1; i <= Math.max(lines, 8); i++) {
            lineHTML += i + '<br>';
        }
        lineNumbers.innerHTML = lineHTML;
    }

    // Scroll Sync
    codeArea.addEventListener('scroll', () => {
        lineNumbers.scrollTop = codeArea.scrollTop;
    });

    // Language Navigation Handler
    languageSelect.addEventListener('change', () => {
        const selected = languageSelect.value;
        if (selected === 'cpp') window.location.href = 'cpp_ui.html';
        else if (selected === 'java') window.location.href = 'java_ui.html';
        else if (selected === 'python') window.location.href = 'python_ui.html';
    });

    // Run C Code Logic
    runBtn.addEventListener('click', async () => {
        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#27ae60';
        consoleOutput.innerHTML = `&gt; Compiling and executing C code...<br>&gt; `;

        try {
            const response = await fetch(C_BACKEND_URL, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ code: codeArea.value })
            });

            const result = await response.json();

            if (result.output) {
                consoleOutput.innerHTML = `&gt; Output:<br>${result.output}`;
            } else if (result.error) {
                consoleOutput.style.color = '#e74c3c';
                consoleOutput.innerHTML = `&gt; Error:<br>${result.error}`;
            }
        } catch (error) {
            consoleOutput.style.color = '#e74c3c';
            consoleOutput.innerHTML = `&gt; Connection Error: Unable to reach C Compiler backend.<br>${error.message}`;
        }
    });

    // Controls
    document.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key === '`') {
            e.preventDefault();
            consoleWrapper.classList.toggle('hidden');
            consoleWrapper.classList.remove('collapsed');
        }
    });

    toggleTerminalBtn.addEventListener('click', () => consoleWrapper.classList.toggle('collapsed'));
    closeTerminalBtn.addEventListener('click', () => consoleWrapper.classList.add('hidden'));
    codeArea.addEventListener('input', updateLineNumbers);
});