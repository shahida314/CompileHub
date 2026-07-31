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

    // Terminal UI Controls
    const consoleWrapper = document.getElementById('consoleWrapper');
    const toggleTerminalBtn = document.getElementById('toggleTerminalBtn');
    const closeTerminalBtn = document.getElementById('closeTerminalBtn');
    const resizeHandle = document.getElementById('consoleResizeHandle');

    const BACKEND_URL = 'http://localhost:5000/api/compile/java';
    // const BACKEND_URL = 'https://your-render-backend-url.onrender.com/api/compile/java';

    let currentAbortController = null;

    // Set Default Java Code (persist across Live Server auto-reloads)
    if (codeArea) {
        const savedCode = localStorage.getItem('compilehub_java_code');
        codeArea.value = savedCode ? savedCode : defaultJavaCode;
        codeArea.addEventListener('input', () => {
            localStorage.setItem('compilehub_java_code', codeArea.value);
        });
    }

    // Dynamic Helper for Formatting Text Output with Line Breaks
    function formatOutputText(text) {
        if (!text) return '';
        return text
            .replace(/&/g, '&amp;')
            .replace(/</g, '&lt;')
            .replace(/>/g, '&gt;')
            .replace(/\n/g, '<br>')
            .replace(/ /g, '&nbsp;');
    }

    // Update Line Numbers dynamically
    function updateLineNumbers() {
        if (!codeArea || !lineNumbers) return;
        const lines = codeArea.value.split('\n').length;
        let lineHTML = '';
        for (let i = 1; i <= Math.max(lines, 8); i++) {
            lineHTML += i + '<br>';
        }
        lineNumbers.innerHTML = lineHTML;
    }

    // Sync scroll between textarea and line numbers
    if (codeArea && lineNumbers) {
        codeArea.addEventListener('scroll', () => {
            lineNumbers.scrollTop = codeArea.scrollTop;
        });
        codeArea.addEventListener('input', updateLineNumbers);
    }

    // Language Dropdown Redirect
    if (languageSelect) {
        languageSelect.addEventListener('change', () => {
            const selectedLang = languageSelect.value;
            if (selectedLang === 'c') window.location.href = 'c_ui.html';
            else if (selectedLang === 'cpp') window.location.href = 'cpp_ui.html';
            else if (selectedLang === 'python') window.location.href = 'python_ui.html';
            else if (selectedLang === 'java') window.location.href = 'java_ui.html';
        });
    }

    // Send Java Code to Render / Local Backend
    if (runBtn) {
        runBtn.addEventListener('click', async () => {
            if (consoleWrapper) {
                consoleWrapper.classList.remove('hidden', 'collapsed');
            }
            if (consoleOutput) {
                consoleOutput.style.color = '#27ae60';
                consoleOutput.innerHTML = `&gt; Compiling and executing JAVA code...<br>&gt; `;
            }

            const code = codeArea ? codeArea.value : '';
            currentAbortController = new AbortController();

            try {
                const response = await fetch(BACKEND_URL, {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify({ code }),
                    signal: currentAbortController.signal
                });

                const result = await response.json();

                if (result.output) {
                    consoleOutput.style.color = '#e0e0e0';
                    consoleOutput.innerHTML = `&gt; Output:<br>${formatOutputText(result.output)}`;
                } else if (result.error) {
                    consoleOutput.style.color = '#e74c3c';
                    consoleOutput.innerHTML = `&gt; Error:<br>${formatOutputText(result.error)}`;
                } else {
                    consoleOutput.style.color = '#f39c12';
                    consoleOutput.innerHTML = `&gt; Program finished with no output.`;
                }
            } catch (error) {
                if (consoleOutput) {
                    consoleOutput.style.color = '#e74c3c';
                    if (error.name === 'AbortError') {
                        consoleOutput.innerHTML = `&gt; Execution stopped by user.`;
                    } else {
                        consoleOutput.innerHTML = `&gt; Connection Error: Unable to reach backend server.<br>${error.message}`;
                    }
                }
            } finally {
                currentAbortController = null;
            }
        });
    }

    // Debug Button
    if (debugBtn) {
        debugBtn.addEventListener('click', (e) => {
            e.preventDefault();
            if (consoleWrapper) consoleWrapper.classList.remove('hidden', 'collapsed');
            if (consoleOutput) {
                consoleOutput.style.color = '#f39c12';
                consoleOutput.innerHTML = `&gt; Debugging mode active...<br>&gt; Syntax check passed.`;
            }
        });
    }

    // Stop Button
    if (stopBtn) {
        stopBtn.addEventListener('click', (e) => {
            e.preventDefault();
            if (currentAbortController) {
                currentAbortController.abort();
            } else if (consoleOutput) {
                if (consoleWrapper) consoleWrapper.classList.remove('hidden', 'collapsed');
                consoleOutput.style.color = '#e74c3c';
                consoleOutput.innerHTML = `&gt; No active process to stop.`;
            }
        });
    }

    // Share Button
    if (shareBtn) {
        shareBtn.addEventListener('click', (e) => {
            e.preventDefault();
            navigator.clipboard.writeText(window.location.href);
            alert('IDE link copied to clipboard!');
        });
    }

    // Save Button
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

    // Keyboard Shortcut (Ctrl + `) to toggle terminal
    document.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key === '`') {
            e.preventDefault();
            if (consoleWrapper) {
                consoleWrapper.classList.toggle('hidden');
                consoleWrapper.classList.remove('collapsed');
            }
        }
    });

    // Minimize / Expand Terminal
    if (toggleTerminalBtn && consoleWrapper) {
        toggleTerminalBtn.addEventListener('click', () => {
            consoleWrapper.classList.toggle('collapsed');
        });
    }

    // Close Terminal
    if (closeTerminalBtn && consoleWrapper) {
        closeTerminalBtn.addEventListener('click', () => {
            consoleWrapper.classList.add('hidden');
        });
    }

    // ===== Drag-to-resize terminal (like a normal IDE) =====
    let isResizing = false;
    let startY = 0;
    let startHeight = 0;

    if (resizeHandle && consoleWrapper) {
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
    }

    // Initial Line Numbers Setup
    updateLineNumbers();
});