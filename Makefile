pft_app: main.cpp FinanceSystem.cpp
	g++ main.cpp FinanceSystem.cpp -o pft_app -std=c++11 `mysql_config --cflags --libs`

clean:
	rm -f pft_app BankStatement.txt

run: clean pft_app
	./pft_app