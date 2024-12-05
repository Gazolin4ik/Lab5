#include <iostream>  
#include <fstream>   
#include <string>    
#include <regex>     

using namespace std; 

// Базовый класс для преобразования
class CodeFormatter {
public:
    virtual ~CodeFormatter() = default; // Виртуальный деструктор для безопасного удаления
    virtual string format(const string& code) = 0; // Чисто виртуальная функция форматирования
};

// Конкретный класс для базового форматирования C++ в HTML
class BasicCodeFormatter : public CodeFormatter {
public:
    string format(const string& code) override {  // Реализация функции форматирования
        return "<pre>" + code + "</pre>";  // Оборачиваем код в тег <pre> для отображения в HTML
    }
};

// Декоратор для добавления стилей для ключевых слов
class KeywordDecorator : public CodeFormatter {
private:
    CodeFormatter* wrappedFormatter;  // Указатель на объект CodeFormatter, который будет декорирован
public:
    explicit KeywordDecorator(CodeFormatter* formatter) : wrappedFormatter(formatter) {}  // Конструктор

    string format(const string& code) override {  // Реализация форматирования
        string formattedCode = wrappedFormatter->format(code);  // Форматируем код с помощью вложенного форматтера
        // Ищем ключевые слова C++ и выделяем их цветом
        regex keywordRegex(R"(\b(int|float|double|if|else|for|while|return|class|public|private|protected|void|const)\b)");
        formattedCode = regex_replace(formattedCode, keywordRegex, "<span style='color: blue;'>$&</span>");  // Заменяем ключевые слова на HTML синим цветом
        return formattedCode;
    }
};

// Декоратор для добавления стилей для комментариев
class CommentDecorator : public CodeFormatter {
private:
    CodeFormatter* wrappedFormatter;  // Указатель на объект CodeFormatter
public:
    explicit CommentDecorator(CodeFormatter* formatter) : wrappedFormatter(formatter) {}  // Конструктор

    string format(const string& code) override {  // Реализация форматирования
        string formattedCode = wrappedFormatter->format(code);  // Форматируем код с помощью вложенного форматтера
        // Ищем однострочные комментарии
        regex singleLineCommentRegex(R"(//.*$)");
        formattedCode = regex_replace(formattedCode, singleLineCommentRegex, "<span style='color: green;'>$&</span>");  // Заменяем однострочные комментарии на HTML с зеленым цветом

        // Ищем многострочные комментарии
        regex multiLineCommentRegex(R"(/\\*.*?\\*/)"); 
        formattedCode = regex_replace(formattedCode, multiLineCommentRegex, "<span style='color: green;'>$&</span>");  // Заменяем многострочные комментарии на HTML с зеленым цветом
        
        return formattedCode;
    }
};

// Декоратор для добавления стилей для строк
class StringLiteralDecorator : public CodeFormatter {
private:
    CodeFormatter* wrappedFormatter;  // Указатель на объект CodeFormatter
public:
    explicit StringLiteralDecorator(CodeFormatter* formatter) : wrappedFormatter(formatter) {}  // Конструктор

    string format(const string& code) override {  // Реализация форматирования
        string formattedCode = wrappedFormatter->format(code);  // Форматируем код с помощью вложенного форматтера
        // Ищем строковые литералы
        regex stringRegex(R"(".*?")");
        formattedCode = regex_replace(formattedCode, stringRegex, "<span style='color: red;'>$&</span>");  // Заменяем строковые литералы на HTML с красным цветом
        return formattedCode;
    }
};

// Декоратор для замены символов < и > в директивах #include
class IncludeDecorator : public CodeFormatter {
private:
    CodeFormatter* wrappedFormatter;  // Указатель на объект CodeFormatter
public:
    explicit IncludeDecorator(CodeFormatter* formatter) : wrappedFormatter(formatter) {}  // Конструктор

    string format(const string& code) override {  // Реализация форматирования
        string formattedCode = wrappedFormatter->format(code);  // Форматируем код с помощью вложенного форматтера
        // Ищем директивы #include и заменяем < и >
        regex includeRegex(R"(#include\s*<([^>]+)>)");
        formattedCode = regex_replace(formattedCode, includeRegex, "#in-clude &lt;$1&gt;");  // Заменяем символы < и > на HTML-сущности
        return formattedCode;
    }
};

int main() {
    // Открываем файл с исходным C++ кодом
    ifstream inputFile("input.cpp");  // Создаем объект для чтения файла
    if (!inputFile.is_open()) {  // Проверяем, открылся ли файл
        cerr << "Не удалось открыть файл input.cpp!" << endl;  // Если файл не открылся, выводим сообщение об ошибке
        return 1;  // Возвращаем ошибку
    }

    // Читаем код из файла
    string code((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());  // Считываем весь код в строку
    inputFile.close();  // Закрываем файл

    // Создаем базовый форматтер
    CodeFormatter* formatter = new BasicCodeFormatter();  // Создаем объект для базового форматирования

    // Добавляем декораторы
    formatter = new KeywordDecorator(formatter);  // Добавляем декоратор для ключевых слов
    formatter = new CommentDecorator(formatter);  // Добавляем декоратор для комментариев
    formatter = new StringLiteralDecorator(formatter);  // Добавляем декоратор для строковых литералов
    formatter = new IncludeDecorator(formatter);  // Добавляем декоратор для директив #include

    // Преобразуем код в HTML с синтаксическим выделением
    string htmlCode = formatter->format(code);  // Форматируем код

    // Записываем результат в файл
    ofstream outputFile("output.html");  // Создаем объект для записи в файл
    if (!outputFile.is_open()) {  // Проверяем, открылся ли файл
        cerr << "Не удалось открыть файл output.html!" << endl;  // Если файл не открылся, выводим сообщение об ошибке
        return 1;  // Возвращаем ошибку
    }

    outputFile << htmlCode;  // Записываем отформатированный код в файл
    outputFile.close();  // Закрываем файл

    // Выводим HTML из файла
    ifstream resultFile("output.html");  // Открываем файл с результатом
    string resultHtml((istreambuf_iterator<char>(resultFile)), istreambuf_iterator<char>());  // Считываем содержимое файла
    cout << resultHtml << endl;  // Выводим результат в консоль

    // Освобождаем память
    delete formatter;  // Удаляем объект formatter, чтобы освободить память

    return 0;  // Завершаем программу
}

