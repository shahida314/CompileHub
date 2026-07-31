const express = require('express');
const cors = require('cors');
const path = require('path');
const { runCCode } = require('./c_hub/c_runner');
const { runJavaCompiler } = require('./java_hub/java_runner');
const { runPythonCode } = require('./python_hub/python_runner');

const app = express();
app.use(cors());
app.use(express.json());

// Serve frontend static files (so Live Server isn't needed)
app.use(express.static(path.join(__dirname, '../frontened')));

// C Compiler API Endpoint
app.post('/api/compile/c', (req, res) => {
    const { code } = req.body;
    runCCode(code, (result) => {
        res.json(result);
    });
});

// Java Code Execution Endpoint
app.post('/api/compile/java', (req, res) => {
    const { code } = req.body;

    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    runJavaCompiler(code, (output) => {
        res.json({ output });
    });
});

// Python Compiler API Endpoint
app.post('/api/compile/python', (req, res) => {
    const { code } = req.body;

    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    runPythonCode(code, (result) => {
        res.json(result);
    });
});

const PORT = process.env.PORT || 5000;
app.listen(PORT, () => console.log(`Backend Server listening on port ${PORT}`));