.PHONY: clean All

All:
	@echo "----------Building project:[ Practica2 - Debug ]----------"
	@"$(MAKE)" -f  "Practica2.mk"
clean:
	@echo "----------Cleaning project:[ Practica2 - Debug ]----------"
	@"$(MAKE)" -f  "Practica2.mk" clean
