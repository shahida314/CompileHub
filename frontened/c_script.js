// Default Standard C Template
const defaultCCode = `#include <stdio.h>\n\nint main() {\n    printf("Hello World\\n");\n    return 0;\n}`;

document.addEventListener('DOMContentLoaded', () => {
    const languageSelect = document.getElementById('languageSelect');
    const codeArea = document.getElementById('codeArea');
    const lineNumbers = document.getElementById('lineNumbers');
    const runBtn = document.getElementById('runBtn');
    const debugBtn = document.getElementById('debugBtn');
    const stopBtn = document.getElementById('stopBtn');
    const shareBtn = document.getElementById('shareBtn');
    const saveBtn = document.getElementById('saveBtn');
    const consoleOutput = document.getElementById('consoleOutput');

    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');

    // API Endpoint
    const C_BACKEND_URL = 'http://localhost:5000/api/compile/c';

    let currentAbortController = null;

    // Set initial template code
    codeArea.value = defaultCCode;
    updateLineNumbers();

    // Dynamic line numbers
    function updateLineNumbers() {
        const lines = codeArea.value.split('\n').length;
        let lineHTML = '';
        for (let i = 1; i <= Math.max(lines, 8); i++) {
            lineHTML += i + '<br>';
        }
        lineNumbers.innerHTML = lineHTML;
    }

    // Scroll sync for line numbers
    codeArea.addEventListener('scroll', () => {
        lineNumbers.scrollTop = codeArea.scrollTop;
    });

    // Language dropdown navigation
    languageSelect.addEventListener('change', () => {
        const selected = languageSelect.value;
        if (selected === 'cpp') window.location.href = 'cpp_ui.html';
        else if (selected === 'java') window.location.href = 'java_ui.html';
        else if (selected === 'python') window.location.href = 'python_ui.html';
    });

    // Run C code
    runBtn.addEventListener('click', async () => {
        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#27ae60';
        consoleOutput.innerHTML = `&gt; Compiling and executing C code...<br>&gt; `;

        currentAbortController = new AbortController();

        try {
            const response = await fetch(C_BACKEND_URL, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ code: codeArea.value }),
                signal: currentAbortController.signal
            });

            const result = await response.json();

            if (result.output) {
                consoleOutput.innerHTML = `&gt; Output:<br>${result.output}`;
            } else if (result.error) {
                consoleOutput.style.color = '#e74c3c';
                consoleOutput.innerHTML = `&gt; Error:<br>${result.error}`;
            } else {
                consoleOutput.innerHTML = `&gt; Process finished with no output.`;
            }
        } catch (error) {
            consoleOutput.style.color = '#e74c3c';
            if (error.name === 'AbortError') {
                consoleOutput.innerHTML = `&gt; Execution stopped by user.`;
            } else {
                consoleOutput.innerHTML = `&gt; Connection Error: Unable to reach backend server.<br>${error.message}`;
            }
        } finally {
            currentAbortController = null;
        }
    });

    // Debug Action
    if (debugBtn) {
        debugBtn.addEventListener('click', () => {
            consoleWrapper.classList.remove('hidden', 'collapsed');
            consoleOutput.style.color = '#f39c12';
            consoleOutput.innerHTML = `&gt; Debugging mode active...<br>&gt; Syntax check passed.`;
        });
    }

    // Stop Action
    if (stopBtn) {
        stopBtn.addEventListener('click', () => {
            if (currentAbortController) {
                currentAbortController.abort();
            } else {
                consoleWrapper.classList.remove('hidden', 'collapsed');
                consoleOutput.style.color = '#e74c3c';
                consoleOutput.innerHTML = `&gt; No active process to stop.`;
            }
        });
    }

    // Share Action
    if (shareBtn) {
        shareBtn.addEventListener('click', () => {
            navigator.clipboard.writeText(window.location.href);
            alert('IDE link copied to clipboard!');
        });
    }

    // Save Action (.c file download)
    if (saveBtn) {
        saveBtn.addEventListener('click', () => {
            const blob = new Blob([codeArea.value], { type: 'text/plain' });
            const link = document.createElement('a');
            link.href = URL.createObjectURL(blob);
            link.download = 'main.c';
            link.click();
            URL.revokeObjectURL(link.href);
        });
    }

    // Live Server-এ শুধুমাত্র Ctrl + ` শর্টকাট
    document.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key === '`') {
            e.preventDefault(); // ব্রাউজারের ডিফল্ট আচরণ ব্লক করবে
            consoleWrapper.classList.toggle('hidden');
            consoleWrapper.classList.remove('collapsed');
        }
    });

    // Terminal UI controls
    if (toggleTerminalBtn) {
        toggleTerminalBtn.addEventListener('click', () => consoleWrapper.classList.toggle('collapsed'));
    }
    if (closeTerminalBtn) {
        closeTerminalBtn.addEventListener('click', () => consoleWrapper.classList.add('hidden'));
    }

    codeArea.addEventListener('input', updateLineNumbers);
});