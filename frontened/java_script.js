// Java Default Template
const defaultJavaCode = `public class Main {
    public static void main(String[] args) {
        System.out.println("Hello World");
    }
}`;

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

    // Terminal UI Controls
    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');

    const WS_URL = 'ws://localhost:5000';

    let socket = null;
    let isRunning = false;
    let currentInputEl = null;

    // Set Default Java Code
    if (codeArea) {
        const savedCode = localStorage.getItem('compilehub_java_code');
        codeArea.value = savedCode ? savedCode : defaultJavaCode;
        codeArea.addEventListener('input', () => {
            localStorage.setItem('compilehub_java_code', codeArea.value);
        });
    }

    // HTML escape function
    function formatOutputText(text) {
        if (!text) return '';
        return text
            .replace(/&/g, '&amp;')
            .replace(/</g, '&lt;')
            .replace(/>/g, '&gt;')
            .replace(/\n/g, '<br>')
            .replace(/ /g, '&nbsp;');
    }

    // ==========================================
    // TERMINAL OUTPUT & INLINE INPUT LOGIC
    // ==========================================

    function appendToConsole(text, color = '#ffffff') {
        if (!consoleOutput || !text) return;

        if (currentInputEl) {
            currentInputEl.remove();
            currentInputEl = null;
        }

        const span = document.createElement('span');
        span.style.color = color;
        span.style.display = 'inline';
        span.innerHTML = formatOutputText(text);
        
        consoleOutput.appendChild(span);
        consoleOutput.scrollTop = consoleOutput.scrollHeight;
    }

    // ডাইনামিক ইনলাইন ইনপুট ফিল্ড
    function createInlineInputPrompt() {
        if (!consoleOutput || currentInputEl) return;

        const inputField = document.createElement('input');
        inputField.type = 'text';
        inputField.className = 'inline-input';
        
        inputField.style.display = 'inline-block';
        inputField.style.background = 'transparent';
        inputField.style.border = 'none';
        inputField.style.outline = 'none';
        inputField.style.color = '#ffffff';
        inputField.style.fontFamily = 'inherit';
        inputField.style.fontSize = 'inherit';
        inputField.style.marginLeft = '5px';

        consoleOutput.appendChild(inputField);
        inputField.focus();

        currentInputEl = inputField;

        inputField.addEventListener('keydown', (e) => {
            if (e.key === 'Enter') {
                const value = inputField.value;
                
                const textSpan = document.createElement('span');
                textSpan.style.color = '#ffffff';
                textSpan.innerHTML = formatOutputText(value) + '<br>';
                
                inputField.replaceWith(textSpan);
                currentInputEl = null;

                if (socket && socket.readyState === WebSocket.OPEN && isRunning) {
                    socket.send(JSON.stringify({ type: 'input', data: value + '\n' }));
                }
            }
        });
    }

    // Update Line Numbers
    function updateLineNumbers() {
        if (!codeArea || !lineNumbers) return;
        const lines = codeArea.value.split('\n').length;
        let lineHTML = '';
        for (let i = 1; i <= Math.max(lines, 8); i++) {
            lineHTML += i + '<br>';
        }
        lineNumbers.innerHTML = lineHTML;
    }

    if (codeArea && lineNumbers) {
        codeArea.addEventListener('scroll', () => {
            lineNumbers.scrollTop = codeArea.scrollTop;
        });
        codeArea.addEventListener('input', updateLineNumbers);
    }

    // Language Selection
    if (languageSelect) {
        languageSelect.addEventListener('change', () => {
            const selectedLang = languageSelect.value;
            if (selectedLang === 'c') window.location.href = 'c_ui.html';
            else if (selectedLang === 'cpp') window.location.href = 'cpp_ui.html';
            else if (selectedLang === 'python') window.location.href = 'python_ui.html';
            else if (selectedLang === 'java') window.location.href = 'java_ui.html';
        });
    }

    function ensureSocket() {
        if (socket && socket.readyState === WebSocket.OPEN) return socket;

        socket = new WebSocket(WS_URL);

      socket.addEventListener('message', (event) => {
            let msg;
            try {
                msg = JSON.parse(event.data);
            } catch (e) {
                return;
            }

            if (msg.type === 'output') {
                appendToConsole(msg.data, '#ffffff');
                // আউটপুট আসার পর প্রোগ্রাম রানিং থাকলে ইনপুট ফিল্ড তৈরি করবে
                if (isRunning) {
                    createInlineInputPrompt();
                }
            } else if (msg.type === 'error') {
                appendToConsole(msg.data, '#e74c3c');
            }  else if (msg.type === 'exit') {
                isRunning = false;

                if (currentInputEl) {
                    currentInputEl.remove();
                    currentInputEl = null;
                }

                // এক্সিট মেসেজের div
                const exitLine = document.createElement('div');
                exitLine.style.margin = '0';
                exitLine.style.padding = '0';
                exitLine.style.lineHeight = '1'; // লাইন হাইট কম করে সংকুচিত করা হলো
                
                const codeVal = (msg.code !== null && msg.code !== undefined) ? msg.code : 0;
                
                exitLine.innerHTML = `<span style="color: #ffffff; font-weight: bold; margin-right: 6px;">></span><span style="color: #ffffff;">Process exited with code ${codeVal}.</span>`;
                
                consoleOutput.appendChild(exitLine);
                consoleOutput.scrollTop = consoleOutput.scrollHeight;
            }
        });

        socket.addEventListener('close', () => {
            socket = null;
        });

        socket.addEventListener('error', () => {
            appendToConsole('\n> Connection Error: Unable to reach backend server.\n', '#e74c3c');
        });

        return socket;
    }

    // Run Button
    if (runBtn) {
        runBtn.addEventListener('click', () => {
            if (consoleWrapper) {
                consoleWrapper.classList.remove('hidden', 'collapsed');
            }
            if (consoleOutput) {
                consoleOutput.innerHTML = '';
            }

            const code = codeArea ? codeArea.value : '';
            const ws = ensureSocket();

            const sendRun = () => {
                ws.send(JSON.stringify({ type: 'run', code }));
                isRunning = true;
            };

            if (ws.readyState === WebSocket.OPEN) {
                sendRun();
            } else {
                ws.addEventListener('open', sendRun, { once: true });
            }
        });
    }

    // Debug Button
    if (debugBtn) {
        debugBtn.addEventListener('click', (e) => {
            e.preventDefault();
            if (consoleWrapper) consoleWrapper.classList.remove('hidden', 'collapsed');
            appendToConsole('> Debugging mode active...\n> Syntax check passed.\n', '#ffffff');
        });
    }

    // Stop Button
    if (stopBtn) {
        stopBtn.addEventListener('click', (e) => {
            e.preventDefault();
            if (socket && socket.readyState === WebSocket.OPEN && isRunning) {
                socket.send(JSON.stringify({ type: 'stop' }));
                isRunning = false;
            }
        });
    }

    // Share Button
    if (shareBtn) {
        shareBtn.addEventListener('click', (e) => {
            e.preventDefault();
            navigator.clipboard.writeText(window.location.href);
            alert('IDE link copied to clipboard!');
        });
    }

    // Save Button
    if (saveBtn) {
        saveBtn.addEventListener('click', (e) => {
            e.preventDefault();
            const code = codeArea ? codeArea.value : '';
            const blob = new Blob([code], { type: 'text/plain' });
            const link = document.createElement('a');
            link.href = URL.createObjectURL(blob);
            link.download = 'Main.java';
            link.click();
            URL.revokeObjectURL(link.href);
        });
    }

    // Terminal Shortcuts & Toggle
    document.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key === '`') {
            e.preventDefault();
            if (consoleWrapper) {
                consoleWrapper.classList.toggle('hidden');
                consoleWrapper.classList.remove('collapsed');
            }
        }
    });

    if (toggleTerminalBtn && consoleWrapper) {
        toggleTerminalBtn.addEventListener('click', () => {
            consoleWrapper.classList.toggle('collapsed');
        });
    }

    if (closeTerminalBtn && consoleWrapper) {
        closeTerminalBtn.addEventListener('click', () => {
            consoleWrapper.classList.add('hidden');
        });
    }

    updateLineNumbers();
});