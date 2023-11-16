FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error opening file");
		return -1;
	}
	char line[256];
	int line_number;
	while (fgets(line, sizeof(line), file) != NULL)
	{
		line_number = strtoimax(line, NULL, 0);
		for (int i = 0; i < 4; ++i)
		{
			memory[pc + i] = (line_number >> (8 * (3 - i))) & 0xFF; // 메모리에 할당
		}
		pc += 4;
		printf("%d\n", line_number);
	}
	line_number = 0xffffffff; // halt
	for (int i = 0; i < 4; ++i)
	{
		memory[pc + i] = (line_number >> (8 * (3 - i))) & 0xFF; // 메모리에 할당
	}

	fclose(file);
	return -EINVAL;