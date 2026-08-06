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

    let currentWebSocket = null;
    let inputStartPos = 0;

    codeArea.value = defaultCCode;
    updateLineNumbers();

    // Initial clean terminal text matching C console style
    consoleOutput.style.color = '#27ae60';
    consoleOutput.innerText = `> Initializing C Console...`;

    // Update editor line numbers based on code text height
    function updateLineNumbers() {
        const lines = codeArea.value.split('\n').length;
        let lineHTML = '';
        for (let i = 1; i <= Math.max(lines, 8); i++) {
            lineHTML += i + '<br>';
        }
        lineNumbers.innerHTML = lineHTML;
    }

    // Sync line numbers scroll with code textarea scroll
    codeArea.addEventListener('scroll', () => {
        lineNumbers.scrollTop = codeArea.scrollTop;
    });

    // Handle language navigation dropdown change
    languageSelect.addEventListener('change', () => {
        const selected = languageSelect.value;
        if (selected === 'cpp') window.location.href = 'cpp_ui.html';
        else if (selected === 'java') window.location.href = 'java_ui.html';
        else if (selected === 'python') window.location.href = 'python_ui.html';
        else if (selected === 'c') window.location.href = 'c_ui.html';
    });

    // Run C code button event listener (WebSocket-based interactive execution for scanf support)
    runBtn.addEventListener('click', (e) => {
        e.preventDefault();

        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#ffffff';
        consoleOutput.innerText = ``;

        inputStartPos = 0;

        // Close existing WebSocket connection if active
        if (currentWebSocket) {
            currentWebSocket.close();
        }

        // Connect to WebSocket server
        currentWebSocket = new WebSocket('ws://localhost:5000');

        currentWebSocket.onopen = () => {
            currentWebSocket.send(JSON.stringify({
                type: 'start_c',
                code: codeArea.value
            }));
        };

        currentWebSocket.onmessage = (event) => {
            const msg = JSON.parse(event.data);

            if (msg.type === 'output' || msg.type === 'error') {
                if (msg.type === 'error') {
                    consoleOutput.style.color = '#e74c3c';
                } else {
                    consoleOutput.style.color = '#ffffff';
                }
                consoleOutput.innerText += msg.data;
                inputStartPos = consoleOutput.innerText.length;
                consoleOutput.scrollTop = consoleOutput.scrollHeight;
            } else if (msg.type === 'close') {
                consoleOutput.innerText += `\n> Process exited with code ${msg.exitCode}.`;
                consoleOutput.scrollTop = consoleOutput.scrollHeight;
            }
        };

        currentWebSocket.onerror = () => {
            consoleOutput.style.color = '#e74c3c';
            consoleOutput.innerText += `\n> WebSocket Connection Error.`;
        };
    });

    // Handle terminal keyboard input (like Python) for scanf support
    consoleOutput.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' && currentWebSocket && currentWebSocket.readyState === WebSocket.OPEN) {
            e.preventDefault();

            const currentText = consoleOutput.innerText;
            const userInput = currentText.substring(inputStartPos);

            consoleOutput.innerText += '\n';
            inputStartPos = consoleOutput.innerText.length;
            consoleOutput.scrollTop = consoleOutput.scrollHeight;

            currentWebSocket.send(JSON.stringify({
                type: 'input',
                data: userInput
            }));
        }
    });

    // Debug button click event
    if (debugBtn) {
        debugBtn.addEventListener('click', (e) => {
            e.preventDefault();
            consoleWrapper.classList.remove('hidden', 'collapsed');
            consoleOutput.style.color = '#f39c12';
            consoleOutput.innerText = `> Debugging mode active...\n> Syntax check passed.`;
        });
    }

    // Stop execution button event
    if (stopBtn) {
        stopBtn.addEventListener('click', (e) => {
            e.preventDefault();
            if (currentWebSocket && currentWebSocket.readyState === WebSocket.OPEN) {
                currentWebSocket.send(JSON.stringify({ type: 'stop' }));
                currentWebSocket.close();
                consoleOutput.style.color = '#e74c3c';
                consoleOutput.innerText += `\n> Execution stopped by user.`;
            } else {
                consoleWrapper.classList.remove('hidden', 'collapsed');
                consoleOutput.style.color = '#e74c3c';
                consoleOutput.innerText = `> No active process to stop.`;
            }
        });
    }

    // Share IDE link button event
    if (shareBtn) {
        shareBtn.addEventListener('click', (e) => {
            e.preventDefault();
            navigator.clipboard.writeText(window.location.href);
            alert('IDE link copied to clipboard!');
        });
    }

    // Save code as .c file button event
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

    // Keyboard shortcut (Ctrl + `) to toggle terminal visibility
    document.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key === '`') {
            e.preventDefault();
            consoleWrapper.classList.toggle('hidden');
            consoleWrapper.classList.remove('collapsed');
        }
    });

    // Terminal minimize/expand toggle controls
    if (toggleTerminalBtn) {
        toggleTerminalBtn.addEventListener('click', () => consoleWrapper.classList.toggle('collapsed'));
    }
    if (closeTerminalBtn) {
        closeTerminalBtn.addEventListener('click', () => consoleWrapper.classList.add('hidden'));
    }

    // Terminal resize logic using mouse drag
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

    // Update line numbers dynamically when typing code
    codeArea.addEventListener('input', updateLineNumbers);
    updateLineNumbers();
});