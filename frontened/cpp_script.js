const defaultCppCode = `#include <iostream>
int main() {
    std::cout << "Hello World" << std::endl;
    return 0;
}`;

document.addEventListener('DOMContentLoaded', () => {
    const languageSelect = document.getElementById('languageSelect');
    const codeArea = document.getElementById('codeArea');
    const lineNumbers = document.getElementById('lineNumbers');
    const runBtn = document.getElementById('runBtn');
    const consoleOutput = document.getElementById('consoleOutput');

    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');
    const resizeHandle = document.getElementById('consoleResizeHandle');

    const BACKEND_URL = 'http://localhost:5000/api/compile/cpp';

    codeArea.value = defaultCppCode;

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

    // ব্যাকএন্ডে কোড পাঠানোর মেইন ফাংশন
    async function runCode(inputData = '') {
        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#ffffff';
        
        // রান করার সময় লোডিং মেসেজ
        if (!inputData) {
            consoleOutput.innerHTML = `&gt; Compiling and executing C++ code...<br>`;
        }

        const code = codeArea.value;

        try {
            const response = await fetch(BACKEND_URL, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ code, input: inputData })
            });
            const result = await response.json();
            
            let outputHTML = '';

            if (result.output) {
                consoleOutput.style.color = '#ffffff';
                outputHTML = `&gt; Output:<br>${result.output.replace(/\n/g, '<br>')}`;
            } else if (result.error) {
                consoleOutput.style.color = '#e74c3c';
                outputHTML = `&gt; Error:<br>${result.error.replace(/\n/g, '<br>')}`;
            } else {
                consoleOutput.style.color = '#ffffff';
                outputHTML = `&gt; Program finished with no output.`;
            }

            // আউটপুটের নিচে সাথে সাথে একটি কাস্টম ইনপুট লাইন যোগ করে দেওয়া (যাতে ইনপুট দেওয়া সম্ভব হয়)
            outputHTML += `<br><br>&gt; <input type="text" id="terminalPrompt" placeholder="Type input here and press Enter (if needed)..." autofocus />`;
            
            consoleOutput.innerHTML = outputHTML;

            // ইনপুট ফিল্ড এক্টিভ করা
            attachPromptListener();

        } catch (error) {
            consoleOutput.style.color = '#e74c3c';
            consoleOutput.innerHTML = `&gt; Connection Error: Unable to reach backend server.<br>${error.message}`;
        }
    }

    // টার্মিনালে ইনপুট দিয়ে Enter চাপলে পুনরায় এক্সিকিউট হওয়া
    function attachPromptListener() {
        const terminalPrompt = document.getElementById('terminalPrompt');
        if (terminalPrompt) {
            terminalPrompt.focus();
            terminalPrompt.addEventListener('keydown', (e) => {
                if (e.key === 'Enter') {
                    const userInput = terminalPrompt.value;
                    runCode(userInput);
                }
            });
        }
    }

    // Run বাটনে ক্লিক করলেই সরাসরি রান হবে (ইনপুট থাকুক বা না থাকুক)
    runBtn.addEventListener('click', () => {
        runCode('');
    });

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