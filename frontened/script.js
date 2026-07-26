// Default Starter Code Templates
const codeTemplates = {
    c: `#include <stdio.h>\n\nint main() {\n    printf("Hello World\\n");\n    return 0;\n}`,
    cpp: `#include <iostream>\n\nint main() {\n    std::cout << "Hello World" << std::endl;\n    return 0;\n}`,
    java: `public class Main {\n    public static void main(String[] args) {\n        System.out.println("Hello World");\n    }\n}`,
    python: `print("Hello World")`
};

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

    // Base URL of your Render Backend
    const BACKEND_BASE_URL = 'https://your-render-backend-url.onrender.com/api/compile';

    // Update Starter Code when switching language in the dropdown
    function handleLanguageChange() {
        const selectedLang = languageSelect.value;
        codeArea.value = codeTemplates[selectedLang] || '';
        updateLineNumbers();
    }

    // Update Line Numbers dynamically
    function updateLineNumbers() {
        const lines = codeArea.value.split('\n').length;
        let lineHTML = '';
        for (let i = 1; i <= Math.max(lines, 8); i++) {
            lineHTML += i + '<br>';
        }
        lineNumbers.innerHTML = lineHTML;
    }

    // Sync scroll between text area and line numbers
    codeArea.addEventListener('scroll', () => {
        lineNumbers.scrollTop = codeArea.scrollTop;
    });

    // Handle Language Swap without reloading the page
    languageSelect.addEventListener('change', handleLanguageChange);

    // Run Code Event Handler
    runBtn.addEventListener('click', async () => {
        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#27ae60';

        const selectedLang = languageSelect.value;
        consoleOutput.innerHTML = `&gt; Compiling and executing ${selectedLang.toUpperCase()} code...<br>&gt; `;

        const code = codeArea.value;

        try {
            // Dynamic endpoint selection: /api/compile/c, /api/compile/cpp, /api/compile/java, /api/compile/python
            const response = await fetch(`${BACKEND_BASE_URL}/${selectedLang}`, {
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

    // Minimize / Expand Terminal Panel
    toggleTerminalBtn.addEventListener('click', () => {
        consoleWrapper.classList.toggle('collapsed');
    });

    // Hide Terminal Panel Completely
    closeTerminalBtn.addEventListener('click', () => {
        consoleWrapper.classList.add('hidden');
    });

    // Live update line numbers on code typing
    codeArea.addEventListener('input', updateLineNumbers);

    // Initial setup on page load
    handleLanguageChange();
});