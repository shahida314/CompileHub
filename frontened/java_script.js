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

    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');

    const WS_URL = 'ws://localhost:5000';

    let socket = null;
    let isRunning = false;
    let currentInputEl = null;

    // ============================================================
    // এই ফ্ল্যাগটাই মূল ফিক্স: শেষ যে টেক্সট আউটপুট হয়েছে সেটা newline
    // (\n) দিয়ে শেষ হয়েছিল কিনা তা ট্র্যাক করে। এর উপর ভিত্তি করেই
    // ঠিক হবে ইনপুট বক্স "পাশে পাশে" বসবে নাকি "নিচে নিচে" (নতুন লাইনে)।
    // শুরুতে true রাখা হয়েছে যাতে প্রথম আউটপুট সবসময় নতুন লাইনেই শুরু হয়।
    // ============================================================
    let lastOutputEndsWithNewline = true;

    // ============================================================
    // NEW: পেজ লোড হওয়ার সাথে সাথেই টার্মিনালে initial message
    // দেখানো হবে (Run চাপার আগেই) — python_ui.html-এর মতো
    // ============================================================
    if (consoleOutput) {
        const initLine = document.createElement('div');
        initLine.style.margin = '0';
        initLine.style.padding = '0';
        initLine.style.lineHeight = '1.2';
        initLine.innerHTML = '<span style="color: #ffffff; font-weight: bold; margin-right: 6px;">&gt;</span><span style="color: #27ae60;">Initializing Java Console...</span>';
        consoleOutput.appendChild(initLine);
    }

    if (codeArea) {
        const savedCode = localStorage.getItem('compilehub_java_code');
        codeArea.value = savedCode ? savedCode : defaultJavaCode;
        codeArea.addEventListener('input', () => {
            localStorage.setItem('compilehub_java_code', codeArea.value);
        });
    }

    function formatOutputText(text) {
        if (!text) return '';
        return text
            .replace(/&/g, '&amp;')
            .replace(/</g, '&lt;')
            .replace(/>/g, '&gt;')
            .replace(/\n/g, '<br>')
            .replace(/ /g, '&nbsp;');
    }

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

        // এই আউটপুট টেক্সটটা \n দিয়ে শেষ হয়েছে কিনা — এই অনুযায়ী পরের
        // ইনপুট বক্স পাশে বসবে নাকি নতুন লাইনে বসবে তা ঠিক হবে
        lastOutputEndsWithNewline = text.endsWith('\n');
    }

    // ============================================================
    // ইনপুট বক্স — println (newline সহ prompt) হলে নতুন লাইনে বসবে,
    // print (newline ছাড়া prompt) হলে prompt-এর ঠিক পাশে বসবে।
    // Enter চাপলে (real terminal-এর মতোই) একটা লাইন-ব্রেক তৈরি হবে।
    // ============================================================
    function createInlineInputPrompt() {
        if (!consoleOutput || currentInputEl || !isRunning) return;

        const inputField = document.createElement('input');
        inputField.type = 'text';
        inputField.className = 'inline-input';
        inputField.autocomplete = 'off';

        if (lastOutputEndsWithNewline) {
            // আগের prompt println দিয়ে শেষ হয়েছিল → নতুন লাইনে (নিচে নিচে)
            inputField.style.display = 'block';
            inputField.style.width = '100%';
            inputField.style.marginTop = '2px';
        } else {
            // আগের prompt print দিয়ে শেষ হয়েছিল (কোনো newline নেই) → পাশে পাশে
            inputField.style.display = 'inline-block';
            inputField.style.width = '140px';
            inputField.style.verticalAlign = 'baseline';
        }

        consoleOutput.appendChild(inputField);
        inputField.focus();
        currentInputEl = inputField;

        inputField.addEventListener('keydown', (e) => {
            if (e.key === 'Enter') {
                const value = inputField.value;

                // টাইপ করা মান inline span আকারে বসানো হচ্ছে, যাতে এটা
                // prompt-এর ঠিক পাশে/জায়গায় বসে (block দিয়ে জোর করে
                // নতুন লাইনে ঠেলে দেওয়া হচ্ছে না)
                const textSpan = document.createElement('span');
                textSpan.style.color = '#ffffff';
                textSpan.style.display = 'inline';
                textSpan.innerHTML = formatOutputText(value);

                inputField.replaceWith(textSpan);
                currentInputEl = null;

                // Enter চাপাটাই বাস্তব terminal-এ একটা newline তৈরি করে —
                // তাই এখানে একটা সত্যিকারের line-break যোগ করা হচ্ছে,
                // এবং ফ্ল্যাগ true করে দেওয়া হচ্ছে
                consoleOutput.appendChild(document.createElement('br'));
                lastOutputEndsWithNewline = true;
                consoleOutput.scrollTop = consoleOutput.scrollHeight;

                if (socket && socket.readyState === WebSocket.OPEN && isRunning) {
                    socket.send(JSON.stringify({ type: 'input', data: value + '\n' }));
                }

                // পরের ইনপুট লাগতে পারে ধরে নিয়ে সাথে সাথেই আবার বক্স রেডি
                // রাখা হচ্ছে, backend output-এর অপেক্ষা না করে
                if (isRunning) {
                    setTimeout(createInlineInputPrompt, 30);
                }
            }
        });
    }

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
                if (isRunning) {
                    createInlineInputPrompt();
                }
            } else if (msg.type === 'error') {
                appendToConsole(msg.data, '#e74c3c');
            } else if (msg.type === 'exit') {
                isRunning = false;

                if (currentInputEl) {
                    currentInputEl.remove();
                    currentInputEl = null;
                }

                if (!lastOutputEndsWithNewline) {
                    consoleOutput.appendChild(document.createElement('br'));
                }

                const exitLine = document.createElement('div');
                exitLine.style.margin = '0';
                exitLine.style.padding = '0';
                exitLine.style.lineHeight = '1';

                const codeVal = (msg.code !== null && msg.code !== undefined) ? msg.code : 0;

                exitLine.innerHTML = `<span style="color: #ffffff; font-weight: bold; margin-right: 6px;">></span><span style="color: #ffffff;">Process exited with code ${codeVal}.</span>`;

                consoleOutput.appendChild(exitLine);
                consoleOutput.scrollTop = consoleOutput.scrollHeight;
                lastOutputEndsWithNewline = true;
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

    if (runBtn) {
        runBtn.addEventListener('click', () => {
            if (consoleWrapper) {
                consoleWrapper.classList.remove('hidden', 'collapsed');
            }
            if (consoleOutput) {
                consoleOutput.innerHTML = '';
            }

            lastOutputEndsWithNewline = true;

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

    if (debugBtn) {
        debugBtn.addEventListener('click', (e) => {
            e.preventDefault();
            if (consoleWrapper) consoleWrapper.classList.remove('hidden', 'collapsed');
            appendToConsole('> Debugging mode active...\n> Syntax check passed.\n', '#ffffff');
        });
    }

    if (stopBtn) {
        stopBtn.addEventListener('click', (e) => {
            e.preventDefault();
            if (socket && socket.readyState === WebSocket.OPEN && isRunning) {
                socket.send(JSON.stringify({ type: 'stop' }));
                isRunning = false;
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
            const code = codeArea ? codeArea.value : '';
            const blob = new Blob([code], { type: 'text/plain' });
            const link = document.createElement('a');
            link.href = URL.createObjectURL(blob);
            link.download = 'Main.java';
            link.click();
            URL.revokeObjectURL(link.href);
        });
    }

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

    // ============================================================
    // TERMINAL RESIZE DRAG (আগের ফিক্স অপরিবর্তিত)
    // ============================================================
    const resizeHandle = document.getElementById('consoleResizeHandle');

    if (resizeHandle && consoleWrapper) {
        let isResizing = false;
        let startY = 0;
        let startHeight = 0;

        resizeHandle.addEventListener('mousedown', function (e) {
            e.preventDefault();
            isResizing = true;
            startY = e.clientY;
            startHeight = consoleWrapper.getBoundingClientRect().height;
            consoleWrapper.classList.add('resizing');
            document.body.style.cursor = 'ns-resize';
            document.body.style.userSelect = 'none';
        });

        document.addEventListener('mousemove', function (e) {
            if (!isResizing) return;

            const dy = startY - e.clientY;
            let newHeight = startHeight + dy;
            const maxHeight = window.innerHeight * 0.7;
            const minHeight = 60;

            newHeight = Math.min(Math.max(newHeight, minHeight), maxHeight);
            consoleWrapper.style.height = `${newHeight}px`;
        });

        document.addEventListener('mouseup', function () {
            if (isResizing) {
                isResizing = false;
                consoleWrapper.classList.remove('resizing');
                document.body.style.cursor = 'default';
                document.body.style.userSelect = 'auto';
            }
        });
    }
});