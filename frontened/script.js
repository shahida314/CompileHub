document.addEventListener('DOMContentLoaded', () => {
    const languageSelect = document.getElementById('languageSelect');
    const runBtn = document.getElementById('runBtn');
    const consoleOutput = document.getElementById('consoleOutput');

    // Terminal UI Controls
    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');
    const resizeHandle = document.getElementById('resizeHandle');

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

    // ----- Resizable Terminal (drag top edge to resize) -----
    let isResizing = false;
    let startY = 0;
    let startHeight = 0;

    const MIN_HEIGHT = 100;
    const MAX_HEIGHT_RATIO = 0.8; // don't let terminal eat more than 80% of viewport

    resizeHandle.addEventListener('mousedown', (e) => {
        // Don't allow resizing while collapsed/hidden
        if (consoleWrapper.classList.contains('collapsed') || consoleWrapper.classList.contains('hidden')) {
            return;
        }
        isResizing = true;
        startY = e.clientY;
        startHeight = consoleWrapper.getBoundingClientRect().height;
       document.body.style.userSelect = 'none';
     document.body.style.cursor = 'ns-resize';
    });

    document.addEventListener('mousemove', (e) => {
        if (!isResizing) return;
        const delta = startY - e.clientY;
        let newHeight = startHeight + delta;

        const maxHeight = window.innerHeight * MAX_HEIGHT_RATIO;
        if (newHeight < MIN_HEIGHT) newHeight = MIN_HEIGHT;
        if (newHeight > maxHeight) newHeight = maxHeight;

        consoleWrapper.style.height = newHeight + 'px';
    });

    document.addEventListener('mouseup', () => {
        if (isResizing) {
            isResizing = false;
            document.body.style.userSelect = '';
            document.body.style.cursor = '';
        }
    });
});