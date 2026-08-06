document.addEventListener('DOMContentLoaded', () => {
    const languageSelect = document.getElementById('languageSelect');
    const runBtn = document.getElementById('runBtn');
    const consoleOutput = document.getElementById('consoleOutput');

    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');
    const resizeHandle = document.getElementById('resizeHandle');

    // Handle language page navigation
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

    languageSelect.addEventListener('change', handleLanguageChange);

    // Run button handler and code execution request
    runBtn.addEventListener('click', async (e) => {
        e.preventDefault();

        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#ffffff';
        consoleOutput.innerHTML = `&gt; Running code...<br>&gt; `;

        const codeArea = document.getElementById('codeArea'); // Make sure your textarea has id="codeArea"
        const userCode = codeArea ? codeArea.value : '';

        try {
            const response = await fetch('/run-c', { // Adjust endpoint according to your backend route
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ code: userCode })
            });

            const result = await response.json();

            // Display execution output or errors
            if (result.output) {
                consoleOutput.style.color = '#ffffff';
                consoleOutput.innerHTML = `${result.output.trim()}`;
            } else if (result.error) {
                const formattedError = result.error.replace(/\n/g, '<br>');
                consoleOutput.style.color = '#e74c3c';
                consoleOutput.innerHTML = `&gt; Error:<br>${formattedError}`;
            } else {
                consoleOutput.style.color = '#ffffff';
                consoleOutput.innerHTML = `&gt; Process finished with no output.`;
            }
        } catch (error) {
            consoleOutput.style.color = '#e74c3c';
            consoleOutput.innerHTML = `&gt; Connection Error: Unable to reach backend server.`;
        }
    });

    // Keyboard shortcut (Ctrl + `) to toggle terminal
    document.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key === '`') {
            e.preventDefault();
            consoleWrapper.classList.toggle('hidden');
            consoleWrapper.classList.remove('collapsed');
        }
    });

    // Minimize or expand terminal panel
    toggleTerminalBtn.addEventListener('click', () => {
        consoleWrapper.classList.toggle('collapsed');
    });

    // Hide terminal panel completely
    closeTerminalBtn.addEventListener('click', () => {
        consoleWrapper.classList.add('hidden');
    });

    // Resizable terminal logic
    let isResizing = false;
    let startY = 0;
    let startHeight = 0;

    const MIN_HEIGHT = 100;
    const MAX_HEIGHT_RATIO = 0.8;

    resizeHandle.addEventListener('mousedown', (e) => {
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