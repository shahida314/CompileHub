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
    const consoleOutput = document.getElementById('consoleOutput');

    // Terminal UI Controls
    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');

    // Backend Endpoint
    const BACKEND_URL = 'https://your-render-backend-url.onrender.com/api/compile/java';

    // Set Default Java Code
    codeArea.value = defaultJavaCode;

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

    // Send Java Code to Render Backend
    runBtn.addEventListener('click', async () => {
        consoleWrapper.classList.remove('hidden', 'collapsed');
        consoleOutput.style.color = '#27ae60';
        consoleOutput.innerHTML = `&gt; Compiling and executing JAVA code...<br>&gt; `;

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

    // Event Listener for text editing
    codeArea.addEventListener('input', updateLineNumbers);

    // Initial Line Numbers Setup
    updateLineNumbers();
});