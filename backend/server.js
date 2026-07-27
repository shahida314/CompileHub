const express = require('express');
const cors = require('cors');
const { runCCode } = require('./c_hub/c_runner');

const app = express();
app.use(cors());
app.use(express.json());

// C Compiler API Endpoint
app.post('/api/compile/c', (req, res) => {
    const { code } = req.body;
    runCCode(code, (result) => {
        res.json(result);
    });
});

const PORT = process.env.PORT || 5000;
app.listen(PORT, () => console.log(`Backend Server listening on port ${PORT}`));