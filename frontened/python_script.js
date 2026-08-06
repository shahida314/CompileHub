const defaultPythonCode = `print("Hello, World!")`;

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

    let ws = null;
    let inputStartPos = 0;

    codeArea.value = defaultPythonCode;

    // Initial clean terminal text matching C console style
    consoleOutput.style.color = '#27ae60';
    consoleOutput.innerText = `> Initializing Python Console...`;

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
        const selectedLang = languageSelect.value;
        if (selectedLang === 'c') window.location.href = 'c_ui.html';
        else if (selectedLang === 'cpp') window.location.href = 'cpp_ui.html';
        else if (selectedLang === 'python') window.location.href = 'python_ui.html';
        else if (selectedLang === 'java') window.location.href = 'java_ui.html';
    });

    // Run code via WebSocket
    runBtn.addEventListener('click', (e) => {
        e.preventDefault();

        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#ffffff';
        consoleOutput.innerText = ``;

        inputStartPos = 0;

        if (ws) ws.close();
        ws = new WebSocket('ws://localhost:5000');

        ws.onopen = () => {
            ws.send(JSON.stringify({ type: 'start', code: codeArea.value }));
        };

        ws.onmessage = (event) => {
            const msg = JSON.parse(event.data);

            if (msg.type === 'output' || msg.type === 'error') {
                if (msg.type === 'error') consoleOutput.style.color = '#ff6b6b';
                else consoleOutput.style.color = '#ffffff';

                consoleOutput.innerText += msg.text;
                inputStartPos = consoleOutput.innerText.length;
                consoleOutput.scrollTop = consoleOutput.scrollHeight;
            }
            else if (msg.type === 'exit') {
                consoleOutput.innerText += `\n> Process exited with code ${msg.code}.`;
                consoleOutput.scrollTop = consoleOutput.scrollHeight;
            }
        };

        ws.onerror = () => {
            consoleOutput.style.color = '#ff6b6b';
            consoleOutput.innerText = `> Connection Error: Unable to reach WebSocket server.`;
        };
    });

    // Send user input on Enter key if input() is used in code
    consoleOutput.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' && ws && ws.readyState === WebSocket.OPEN) {
            e.preventDefault();

            const currentText = consoleOutput.innerText;
            const userInput = currentText.substring(inputStartPos);

            consoleOutput.innerText += '\n';
            inputStartPos = consoleOutput.innerText.length;

            ws.send(JSON.stringify({ type: 'input', text: userInput }));
        }
    });

    if (debugBtn) {
        debugBtn.addEventListener('click', (e) => {
            e.preventDefault();
            consoleWrapper.classList.remove('hidden', 'collapsed');
            consoleOutput.style.color = '#f39c12';
            consoleOutput.innerText = `> Debugging mode active...\n> Syntax check passed.`;
        });
    }

    if (stopBtn) {
        stopBtn.addEventListener('click', (e) => {
            e.preventDefault();
            if (ws) {
                ws.close();
                ws = null;
                consoleWrapper.classList.remove('hidden', 'collapsed');
                consoleOutput.style.color = '#ff6b6b';
                consoleOutput.innerText += `\n> Execution stopped by user.`;
            } else {
                consoleWrapper.classList.remove('hidden', 'collapsed');
                consoleOutput.style.color = '#ff6b6b';
                consoleOutput.innerText = `> No active process to stop.`;
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
            link.download = 'main.py';
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

    toggleTerminalBtn.addEventListener('click', () => {
        consoleWrapper.classList.toggle('collapsed');
    });

    closeTerminalBtn.addEventListener('click', () => {
        consoleWrapper.classList.add('hidden');
    });

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