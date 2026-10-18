#define _CRT_SECURE_NO_WARNINGS
#include<locale.h>
#include <stdio.h>
#include <math.h>

#define N 60

/**
 * @brief Вычисляет преобразованное показание Q(X) для мониторинга 
 *
 * @param x Давление сырья (должно быть в диапазоне от 1.0 до 12.0 бар)
 * @param base_q0 Базовое значение Q0 (44.39)
 * @param result_q Указатель на переменную, куда будет записан результат Q(X)
 * @return int 0 в случае успеха, -1 в случае ошибки (выход за диапазон)
 */
int calculate(double x, double base_q0, double* result_q) 
{
    setlocale(LC_ALL, "RUS");
    if (x < 1.0 || x > 12.0) 
    {
        return -1;
    }

    if (x <= 4.0) 
    {
        *result_q = (x - 4.0) / 2.0 + base_q0;
    }
    else if (x > 4.0 && x <= 8.0) 
    {
        *result_q = 4.0 * log(x - 6.0 + 2.0) + 40.0;
    }
    else 
    { 
        *result_q = 3.0 * sqrt(x - 8.0 + 1.0) + 65.0;
    }

    return 0;
}

int main() 
{
    setlocale(LC_ALL, "RUS");
    double x_input;
    double q_output;

    const double base_q0 = 44.39;
    const double min_allowed = base_q0 * 0.95;
    const double max_allowed = base_q0 * 1.05;


    double sub_sum = 0.0;
    int sub_count = 0;
    int above_average_count = 0;


    for (int i = 1; i <= N; i++)
    {
        printf("[%d/%d] ââåäèòå çíà÷åíèå äàâëåíèÿ X (áàð): ", i, N);

        if (scanf("%lf", &x_input) != 1) 
        {
            printf("îøèáêà: òðåáóåòñÿ ÷èñëîâîå çíà÷åíèå.\n\n");
            while (getchar() != '\n');
            i--;
            continue;
        }

        q_output = 0.0;

        int status_code = calculate(x_input, base_q0, &q_output);

        if (status_code == -1) 
        {
            printf("îøèáêà : äàâëåíèå %.2f áàð âûõîäèò çà ðàìêè(1-12 áàð)!\n\n", x_input);
            i--;
            continue;
        }

        if (i >= 11 && i <= 37) 
        {
            sub_count++;
            sub_sum += q_output;

            double current_average = sub_sum / sub_count;

            if (q_output > current_average) 
            {
                above_average_count++;
            }
        }

        printf("ðåçóëüòàò Q(X) = %.4f\n", q_output);

        if (q_output >= min_allowed && q_output <= max_allowed)
        {
            printf("ñòàòóñ: â íîðìå\n");
        }
        else if (q_output < min_allowed)
        {
            printf("ñòàòóñ: îòêëîíåíèå(íèæå) \n");
        }
        else 
        {
            printf("ñòàòóñ: îòêëîíåíèå (âûøå)\n");
        }
        printf("\n");
    }

    double percentage = 0.0;
    if (sub_count > 0) 
    {
        percentage = ((double)above_average_count / sub_count) * 100.0;
    }

    printf("âñåãî ïðîàíàëèçèðîâàíî øàãîâ (ñ 11 ïî 37): %d\n", sub_count);
    printf("èòîãîâîå ñðåäíåå àðèôìåòè÷åñêîå äèàïàçîíà: %.4f\n", sub_count > 0 ? (sub_sum / sub_count) : 0.0);
    printf("êîëè÷åñòâî çíà÷åíèé âûøå ñðåäíåãî: %d\n", above_average_count);
    printf("äîëÿ ýòèõ çíà÷åíèé: %.2f%%\n", percentage);
    
    return 0;
}
