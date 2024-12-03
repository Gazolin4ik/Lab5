#include <iostream>
#include <string>
#include <regex>

// Базовый класс для преобразования
class CodeFormatter {
public:
    virtual ~CodeFormatter() = default;
    virtual std::string format(const std::string& code) = 0;
};

// Конкретный класс для базового форматирования C++ в HTML
class BasicCodeFormatter : public CodeFormatter {
public:
    std::string format(const std::string& code) override {
        return "<pre>" + code + "</pre>";
    }
};

// Декоратор для добавления стилей для ключевых слов
class KeywordDecorator : public CodeFormatter {
private:
    CodeFormatter* wrappedFormatter;
public:
    explicit KeywordDecorator(CodeFormatter* formatter) : wrappedFormatter(formatter) {}

    std::string format(const std::string& code) override {
        std::string formattedCode = wrappedFormatter->format(code);
        // Ищем ключевые слова C++ и выделяем их цветом
        std::regex keywordRegex(R"(\b(int|float|double|if|else|for|while|return|class|public|private|protected|void|const)\b)");
        formattedCode = std::regex_replace(formattedCode, keywordRegex, "<span style='color: blue;'>$&</span>");
        return formattedCode;
    }
};

// Декоратор для добавления стилей для комментариев
class CommentDecorator : public CodeFormatter {
private:
    CodeFormatter* wrappedFormatter;
public:
    explicit CommentDecorator(CodeFormatter* formatter) : wrappedFormatter(formatter) {}

    std::string format(const std::string& code) override {
        std::string formattedCode = wrappedFormatter->format(code);
        // Ищем однострочные комментарии
        std::regex singleLineCommentRegex(R"(//.*$)");
        formattedCode = std::regex_replace(formattedCode, singleLineCommentRegex, "<span style='color: green;'>$&</span>");

        // Ищем многострочные комментарии
        std::regex multiLineCommentRegex(R"(/\\*.*?\\*/)");
        formattedCode = std::regex_replace(formattedCode, multiLineCommentRegex, "<span style='color: green;'>$&</span>");
        
        return formattedCode;
    }
};

// Декоратор для добавления стилей для строк
class StringLiteralDecorator : public CodeFormatter {
private:
    CodeFormatter* wrappedFormatter;
public:
    explicit StringLiteralDecorator(CodeFormatter* formatter) : wrappedFormatter(formatter) {}

    std::string format(const std::string& code) override {
        std::string formattedCode = wrappedFormatter->format(code);
        // Ищем строковые литералы
        std::regex stringRegex(R"(".*?")");
        formattedCode = std::regex_replace(formattedCode, stringRegex, "<span style='color: red;'>$&</span>");
        return formattedCode;
    }
};

int main() {
    std::string code = R"(// This is a comment
int main() {
    // Another comment
    std::cout << "Hello, World!" << std::endl;
    return 0;
})";

    // Создаем базовый форматтер
    CodeFormatter* formatter = new BasicCodeFormatter();

    // Добавляем декораторы
    formatter = new KeywordDecorator(formatter);
    formatter = new CommentDecorator(formatter);
    formatter = new StringLiteralDecorator(formatter);

    // Преобразуем код в HTML с синтаксическим выделением
    std::string htmlCode = formatter->format(code);

    // Выводим результат
    std::cout << htmlCode << std::endl;

    // Освобождаем память
    delete formatter;

    return 0;
}
