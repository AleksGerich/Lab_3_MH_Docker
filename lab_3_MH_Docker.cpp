#include <iostream>
#include <vector>
#include <ctime>
#include <bitset>
#include <fstream>
#include <math.h> 
#include <string>
#include <cstdlib>

using namespace std;


class MH
{
public:
    // Векторный массив с исходным сообщением
    string text;
    // Векторный массив с зашифрованным сообщением
    vector<int> c_text;
    // Векторный массив с расшифрованным сообщением
    vector<uint8_t> enc_text;
    // Векторный массив с текстом в бинарном восьмибитном виде
    vector<int> text_bin;
    // Открытый ключ
    vector<int> op_key;
    // m и n соответственно
    int m, n = 0;



    vector<int> get()
    {
        return pr_key;
    }

    // Функция формирования m и n
    void FormMN()
    {
        // Формирование m
        int temp_counter = 0;
        for (int i = 0; i < pr_key.size(); i++)
        {
            temp_counter += pr_key[i];
        }
        m = temp_counter + 1;

        // Формирование n
        srand(time(0));
        while (true)
        {
            n = 1 + rand() % (m - 1);

            if (n % 2 == 0)
                continue;

            double root = round(sqrt(n));

            bool flag = true;
            for (int i = 2; i < root; i++)
            {
                if (n % i == 0)
                {
                    flag = false;
                }
            }

            if (flag)
                break;
        }
        cout << "m = " << m << endl;
        cout << "n = " << n << endl;
    }

    // Функция формирования открытого ключа
    vector<int> FormOpenKey()
    {
        FormMN();

        vector<int> key;

        for (int i = 0; i < pr_key.size(); i++)
        {
            key.push_back((pr_key[i] * n) % m);
        }
        op_key = key;

        return key;
    }

    // Функция преобразования исходного текста в бинарный вид на основе ASCII кода символов
    void BinConvector(string mes)
    {
        string temp_str_bin;
        int temp_int;

        for (int i = 0; i < mes.size(); i++)
        {
            temp_int = static_cast<int>(mes[i]);

            temp_str_bin = bitset<sizeof(int) * 8>(temp_int).to_string();

            text_bin.push_back(stoi(temp_str_bin));
        }

        for (int i = 0; i < text_bin.size(); i++) {
            cout << "==== BIN " << mes[i] << " ====" << endl;
            cout << text_bin[i] << " " << endl;
        }
        cout << endl;
    }



    // Функция шифрования
    void Encrypt()
    {
        vector<int> c_mes;

        for (int i = 0; i < text_bin.size(); i++)
        {
            // Преобразование из численного вида в string
            string bin_str;

            string temp = to_string(text_bin[i]);

            // Добавление нулей до получения восьмибитного вида
            if (temp.size() != 8)
            {
                int size = temp.size();

                // Заполнение нулями разницы
                for (int j = 0; j < 8 - size; j++)
                    bin_str += "0";

                // Копирование оставшегося
                for (int j = 0; j < temp.size(); j++)
                    bin_str += temp[j];
            }
            else
                bin_str = temp;

            // Процесс шифрования
            int counter = 0;
            for (int j = 0; j < bin_str.size(); j++)
            {
                if (bin_str[j] != '0')
                {
                    counter += op_key[j];
                }
            }
            c_mes.push_back(counter);
        }

        cout << "Crypted message: " << endl;
        for (int i = 0; i < c_mes.size(); i++) {
            cout << c_mes[i] << " ";
        }
        cout << endl;

        c_text = c_mes;
    }

    // Функция подсчета мультипликативного обратного n
    int ModInverse(int m, int n)
    {
        int i = m, v = 0, d = 1;

        while (n > 0)
        {
            int t = i / n, x = n;
            n = i % x;
            i = x;
            x = d;
            d = v - t * x;
            v = x;
        }

        v %= m;
        if (v < 0)
            v = (v + m) % m;
        return v;
    }

    // Функция преобразования из bin в dec
    int binaryToDecimal(int n)
    {
        int num = n;
        int dec_value = 0;
        int base = 1;

        int temp = num;
        while (temp) {
            int last_digit = temp % 10;
            temp = temp / 10;
            dec_value += last_digit * base;
            base = base * 2;
        }

        return dec_value;
    }

    // Функция дешифровки сообщения
    void Decrypt()
    {
        int MI = ModInverse(m, n);

        for (int i = 0; i < c_text.size(); i++)
        {
            cout << "\n====" << " Символ " << i + 1 << "====" << endl;

            int magic = c_text[i] * MI % m;

            vector<int> bin(pr_key.size());

            while (true)
            {
                for (int j = 0; j < pr_key.size(); j++)
                {
                    if (pr_key[j] > magic)
                    {
                        bin[j - 1] = 1;
                        cout << magic << " - " << pr_key[j - 1] << " = " << magic - pr_key[j - 1] << endl;
                        magic = magic - pr_key[j - 1];
                        break;
                    }
                    if (j == pr_key.size() - 1)
                    {
                        bin[j] = 1;
                        cout << magic << " - " << pr_key[j] << " = " << magic - pr_key[j] << endl;
                        magic = magic - pr_key[j];
                    }
                }
                if (magic == 0)
                    break;
            }

            // Конвертирование бинарного числа в string
            string temp;
            for (int j = 0; j < bin.size(); j++)
            {
                temp += to_string(bin[j]);
            }
            cout << "Полученный бинарный символ:" << endl;
            cout << temp << endl;

            // Перевод из двоичной системы в десятичную
            int dec = binaryToDecimal(stoi(temp));

            enc_text.push_back((char)(dec));
        }
    }

private:
    // Векторный массив с закрытым ключом
    vector<int> pr_key = FormPrivateKey();

    // Формирование закрытого ключа
    vector<int> FormPrivateKey()
    {
        vector<int> key;

        srand(time(0));

        int first_value = 1;
        int last_value = 10;

        int random_num;
        int counter = 0;

        for (int i = 0; i < 8; i++)
        {
            random_num = first_value + rand() % last_value;
            key.push_back(random_num);

            counter = 0;
            for (int j = 0; j < key.size(); j++) {
                counter += key[j];
            }

            first_value = counter + 1;
            last_value = first_value + 3;
        }

        return key;
    }
};




void start(string message)
{
    MH obj;

    obj.text = message;

    cout << "Исходный текст: " << endl;
    for (int i = 0; i < obj.text.size(); i++) {
        cout << obj.text[i];
    }
    cout << endl;

    vector<int> temp = obj.get();

    cout << "\nPrivate key: " << endl;
    for (int i = 0; i < 8; i++) {
        cout << temp[i] << " ";
    }
    cout << endl;


    vector<int> temp2 = obj.FormOpenKey();

    cout << "\nOpen key: " << endl;
    for (int i = 0; i < 8; i++) {
        cout << obj.op_key[i] << " ";
    }
    cout << endl;

    cout << "\nПреобразование текста:\n";
    obj.BinConvector(obj.text);

    obj.Encrypt();

    cout << "\nДешифрование:\n";
    obj.Decrypt();

    cout << "\nДешифрованный текст:\n";
    for (int i = 0; i < obj.enc_text.size(); i++)
    {
        cout << obj.enc_text[i];
    }
}



int main()
{
    setlocale(LC_ALL, "Russian");

    start("Test");
}