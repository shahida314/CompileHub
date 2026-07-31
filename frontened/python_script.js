// Python Default Template
const defaultPythonCode = `print("Hello World")`;

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

    const BACKEND_URL = 'http://localhost:5000/api/compile/python';

    codeArea.value = defaultPythonCode;

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

    runBtn.addEventListener('click', async () => {
        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#ffffff';
        consoleOutput.innerHTML = `&gt; Compiling and executing PYTHON code...<br>&gt; `;

        const code = codeArea.value;

        try {
            const response = await fetch(BACKEND_URL, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ code })
            });

            const result = await response.json();

            if (result.output) {
                consoleOutput.style.color = '#ffffff';
                consoleOutput.innerHTML = `&gt; Output:<br>${result.output}`;
            } else if (result.error) {
                consoleOutput.style.color = '#ff6b6b';
                consoleOutput.innerHTML = `&gt; Error:<br>${result.error}`;
            } else {
                consoleOutput.style.color = '#ffffff';
                consoleOutput.innerHTML = `&gt; Program finished with no output.`;
            }
        } catch (error) {
            consoleOutput.style.color = '#ff6b6b';
            consoleOutput.innerHTML = `&gt; Connection Error: Unable to reach backend server.<br>${error.message}`;
        }
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