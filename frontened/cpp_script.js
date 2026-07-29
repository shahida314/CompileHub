// C++ Default Template
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

    // Terminal UI Controls
    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');
    const resizeHandle = document.getElementById('consoleResizeHandle');

    // Backend Endpoint
    const BACKEND_URL = 'https://your-render-backend-url.onrender.com/api/compile/cpp';

    // Set Default C++ Code
    codeArea.value = defaultCppCode;

    // Update Line Numbers dynamically
    function updateLineNumbers() {
        const lines = codeArea.value.split('\n').length;
        let lineHTML = '';
        for (let i = 1; i <= Math.max(lines, 8); i++) {
            lineHTML += i + '<br>';
        }
        lineNumbers.innerHTML = lineHTML;
    }

    // Sync scroll between textarea and line numbers
    codeArea.addEventListener('scroll', () => {
        lineNumbers.scrollTop = codeArea.scrollTop;
    });

    // Language Dropdown Redirect
    languageSelect.addEventListener('change', () => {
        const selectedLang = languageSelect.value;
        if (selectedLang === 'c') window.location.href = 'c_ui.html';
        else if (selectedLang === 'cpp') window.location.href = 'cpp_ui.html';
        else if (selectedLang === 'python') window.location.href = 'python_ui.html';
        else if (selectedLang === 'java') window.location.href = 'java_ui.html';
    });

    // Send C++ Code to Render Backend
    runBtn.addEventListener('click', async () => {
        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#27ae60';
        consoleOutput.innerHTML = `&gt; Compiling and executing C++ code...<br>&gt; `;

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
                consoleOutput.innerHTML = `&gt; Output:<br>${result.output}`;
            } else if (result.error) {
                consoleOutput.style.color = '#e74c3c';
                consoleOutput.innerHTML = `&gt; Error:<br>${result.error}`;
            } else {
                consoleOutput.innerHTML = `&gt; Program finished with no output.`;
            }
        } catch (error) {
            consoleOutput.style.color = '#e74c3c';
            consoleOutput.innerHTML = `&gt; Connection Error: Unable to reach backend server.<br>${error.message}`;
        }
    });

    // Keyboard Shortcut (Ctrl + `) to toggle terminal
    document.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key === '`') {
            e.preventDefault();
            consoleWrapper.classList.toggle('hidden');
            consoleWrapper.classList.remove('collapsed');
        }
    });

    // Minimize / Expand Terminal
    toggleTerminalBtn.addEventListener('click', () => {
        consoleWrapper.classList.toggle('collapsed');
    });

    // Close Terminal
    closeTerminalBtn.addEventListener('click', () => {
        consoleWrapper.classList.add('hidden');
    });

    // ===== Drag-to-resize terminal (like a normal IDE) =====
    let isResizing = false;
    let startY = 0;
    let startHeight = 0;

    resizeHandle.addEventListener('mousedown', (e) => {
        // Don't allow resizing while collapsed/hidden
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

        // Dragging up increases height, dragging down decreases it
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

    // Event Listener for text editing
    codeArea.addEventListener('input', updateLineNumbers);

    // Initial Line Numbers Setup
    updateLineNumbers();
});