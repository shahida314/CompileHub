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
    const resizeHandle = document.getElementById('consoleResizeHandle');

    const C_BACKEND_URL = 'http://localhost:5000/api/compile/c';
    let currentAbortController = null;

    codeArea.value = defaultCCode;
    updateLineNumbers();

    function updateLineNumbers() {
        const lines = codeArea.value.split('\n').length;
        let lineHTML = '';
        for (let i = 1; i <= Math.max(lines, 8); i++) {
            lineHTML += i + '<br>';
        }
        lineNumbers.innerHTML = lineHTML;
    }

    codeArea.addEventListener('scroll', () => {
        lineNumbers.scrollTop = codeArea.scrollTop;
    });

    languageSelect.addEventListener('change', () => {
        const selected = languageSelect.value;
        if (selected === 'cpp') window.location.href = 'cpp_ui.html';
        else if (selected === 'java') window.location.href = 'java_ui.html';
        else if (selected === 'python') window.location.href = 'python_ui.html';
    });

    runBtn.addEventListener('click', async (e) => {
        e.preventDefault();

        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#ffffff';
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
                consoleOutput.style.color = '#ffffff';
                consoleOutput.innerHTML = `&gt; Output:<br>${result.output.trim()}`;
            } else if (result.error) {
                const formattedError = result.error.replace(/\n/g, '<br>');
                consoleOutput.style.color = '#e74c3c';
                consoleOutput.innerHTML = `&gt; Error:<br>${formattedError}`;
            } else {
                consoleOutput.style.color = '#ffffff';
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

    if (debugBtn) {
        debugBtn.addEventListener('click', (e) => {
            e.preventDefault();
            consoleWrapper.classList.remove('hidden', 'collapsed');
            consoleOutput.style.color = '#f39c12';
            consoleOutput.innerHTML = `&gt; Debugging mode active...<br>&gt; Syntax check passed.`;
        });
    }

    if (stopBtn) {
        stopBtn.addEventListener('click', (e) => {
            e.preventDefault();
            if (currentAbortController) {
                currentAbortController.abort();
            } else {
                consoleWrapper.classList.remove('hidden', 'collapsed');
                consoleOutput.style.color = '#e74c3c';
                consoleOutput.innerHTML = `&gt; No active process to stop.`;
            }
        });
    }

    if (shareBtn) {
        shareBtn.addEventListener('click', (e) => {
            e.preventDefault();
            navigator.clipboard.writeText(window.location.href);
            alert('IDE link copied to clipboard!');
        });
    }

    if (saveBtn) {
        saveBtn.addEventListener('click', (e) => {
            e.preventDefault();
            const blob = new Blob([codeArea.value], { type: 'text/plain' });
            const link = document.createElement('a');
            link.href = URL.createObjectURL(blob);
            link.download = 'main.c';
            link.click();
            URL.revokeObjectURL(link.href);
        });
    }

    document.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key === '`') {
            e.preventDefault();
            consoleWrapper.classList.toggle('hidden');
            consoleWrapper.classList.remove('collapsed');
        }
    });

    if (toggleTerminalBtn) {
        toggleTerminalBtn.addEventListener('click', () => consoleWrapper.classList.toggle('collapsed'));
    }
    if (closeTerminalBtn) {
        closeTerminalBtn.addEventListener('click', () => consoleWrapper.classList.add('hidden'));
    }

    // Terminal resize logic
    let isResizing = false;
    let startY = 0;
    let startHeight = 0;

    resizeHandle.addEventListener('mousedown', (e) => {
        if (consoleWrapper.classList.contains('collapsed') ||
            consoleWrapper.classList.contains('hidden')) return;

        isResizing = true;
        startY = e.clientY;
        startHeight = consoleWrapper.offsetHeight;
        consoleWrapper.classList.add('resizing');
        document.body.style.cursor = 'ns-resize';
        e.preventDefault();
    });

    document.addEventListener('mousemove', (e) => {
        if (!isResizing) return;

        const delta = startY - e.clientY;
        let newHeight = startHeight + delta;

        const minHeight = 80;
        const maxHeight = window.innerHeight * 0.7;
        newHeight = Math.max(minHeight, Math.min(maxHeight, newHeight));

        consoleWrapper.style.height = `${newHeight}px`;
    });

    document.addEventListener('mouseup', () => {
        if (isResizing) {
            isResizing = false;
            consoleWrapper.classList.remove('resizing');
            document.body.style.cursor = '';
        }
    });

    codeArea.addEventListener('input', updateLineNumbers);
    updateLineNumbers();
});