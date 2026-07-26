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

    // Handle Language Page Navigation on Dropdown Select
    function handleLanguageChange() {
        const selectedLang = languageSelect.value;

        if (selectedLang === 'c') {
            window.location.href = 'c_ui.html';
        } else if (selectedLang === 'cpp') {
            window.location.href = 'cpp_ui.html';
        } else if (selectedLang === 'java') {
            window.location.href = 'java_ui.html';
        } else if (selectedLang === 'python') {
            window.location.href = 'python_ui.html';
        }
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

    // Listen for language dropdown change
    languageSelect.addEventListener('change', handleLanguageChange);

    // Run Code Warning if no language is selected in index.html
    runBtn.addEventListener('click', () => {
        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#e74c3c';
        consoleOutput.innerHTML = `&gt; Please select a language from the dropdown first!`;
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

    // Initial Line Setup
    updateLineNumbers();
});