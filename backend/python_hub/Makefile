all:
	@echo "Python Hub is ready. No explicit C compilation needed for interpreted Python code."

test:
	python python_runner.js tests/valid.txt
	python python_runner.js tests/invalid.txt

clean:
	@echo "Cleaning up temporary Python compilation files..."
	rm -rf tests/temp_code.py