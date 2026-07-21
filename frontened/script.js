// Default Code Templates 
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

    // Terminal UI Control Elements
    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');

    // Load appropriate starter code when active language changes
    function handleLanguageChange() {
        const selectedLang = languageSelect.value;
        codeArea.value = codeTemplates[selectedLang] || '';
        updateLineNumbers();
    }

    // Update line numbers continuously as lines are typed/deleted
    function updateLineNumbers() {
        const lines = codeArea.value.split('\n').length;
        let lineHTML = '';
        for (let i = 1; i <= Math.max(lines, 8); i++) {
            lineHTML += i + '<br>';
        }
        lineNumbers.innerHTML = lineHTML;
    }

    // Run Button Event: Reveal terminal panel if closed & initiate execution feedback
    runBtn.addEventListener('click', () => {
        consoleWrapper.classList.remove('hidden', 'collapsed');

        const selectedLang = languageSelect.value;
        consoleOutput.innerHTML = `&gt; Compiling and executing ${selectedLang.toUpperCase()} code...<br>&gt; `;
    });

    // Keyboard Shortcut Event: Press (Ctrl + `) to Toggle Terminal Visibility
    document.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key === '`') {
            e.preventDefault();
            consoleWrapper.classList.toggle('hidden');
            consoleWrapper.classList.remove('collapsed');
        }
    });

    // Minimize or Expand Terminal Toggle Button
    toggleTerminalBtn.addEventListener('click', () => {
        consoleWrapper.classList.toggle('collapsed');
    });

    // Hide Terminal Panel Completely
    closeTerminalBtn.addEventListener('click', () => {
        consoleWrapper.classList.add('hidden');
    });

    // Event Listeners for Input & Language Selection
    languageSelect.addEventListener('change', handleLanguageChange);
    codeArea.addEventListener('input', updateLineNumbers);

    // Initialize Page Setup (Defaulting to C Language)
    handleLanguageChange();
});