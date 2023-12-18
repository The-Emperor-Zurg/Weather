#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "conio.h"
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

constexpr int kHours = 24;
size_t day = 0;
constexpr int kRequest = 10;

std::map<size_t, std::string> week = {
    {0, "Sun"},
    {1, "Mon"},
    {2, "Tue"},
    {3, "Wen"},
    {4, "Thu"},
    {5, "Fri"},
    {6, "Sat"}
};

std::map<std::string, std::string> months = {
    {"01", "Jan"},
    {"02", "Feb"},
    {"03", "Mar"},
    {"04", "Apr"},
    {"05", "May"},
    {"06", "Jun"},
    {"07", "Jul"},
    {"08", "Aug"},
    {"09", "Sep"},
    {"10", "Oct"},
    {"11", "Nov"},
    {"12", "Dec"}
};

std::map<size_t, std::vector<std::string>> pictures = {
    {1, {"              weather: ",
         "   .--.       temperature: ",
         " -(    ).     wind: ",
         "(___)__)      precipitation: ",
         "              pressure: "} },

    {2, {"     \\   /    weather: ",
         "      .-.     temperature: ",
         "   - (   ) -  wind: ",
         "      '-'     precipitation: ",
         "     /   \\    pressure: "} },

    {3, {"    .-.       weather: ",
         "   (   ).     temperature: ",
         "  (___()      wind: ",
         "  * * * *     precipitation: ",
         " * * * *      pressure: "} },

    {4, {"              weather: ",
         "-_-_-_-_-_    temperature: ",
         "_-_-_-_-_-    wind: ",
         "-_-_-_-_-_    recipitation: ",
         "              pressure: "} },

    {5, {"    .-.       weather: ",
         "   (   ).     temperature: ",
         "  (___()      wind: ",
         "  *   *       precipitation: ",
         " *   *        pressure: "} },

    {6, {"    .-.       weather: ",
         "   (   ).     temperature: ",
         "  (___()      wind: ",
         "  / / / /     precipitation: ",
         " / / / /      pressure: "} },

    {7, {"    .-.       weather: ",
         "   (   ).     temperature: ",
         "  (___()      wind: ",
         "  /   /       precipitation: ",
         "   /   /      pressure: "} },

    {8, {"    .-.       weather: ",
         "   (   ).     temperature: ",
         "  (___()      wind: ",
         "    \\\\      precipitation: ",
         "     //       pressure: "} }
};

struct weather_block{
    std::string time{};
    int precipitation{};
    size_t precipitation_probability{};
    size_t relativehumidity{};
    int surface_pressure{};
    int temperature{};
    size_t weathercode{};
    size_t winddirection{};
    int windspeed{};
};

void GetInformation(const nlohmann::json& weather, std::vector<weather_block>& vec, const nlohmann::json& conf, const int k) {
    int days = k;
    for (size_t i = 0; i < days * kHours; ++i) {
        weather_block tmp;
        tmp.time = weather["hourly"]["time"][i];
        tmp.precipitation = static_cast<int>(weather["hourly"]["precipitation"][i]);
        tmp.precipitation_probability = weather["hourly"]["precipitation_probability"][i];
        tmp.relativehumidity = weather["hourly"]["relativehumidity_2m"][i];
        tmp.surface_pressure = static_cast<int>(weather["hourly"]["surface_pressure"][i]);
        tmp.temperature = static_cast<int>(weather["hourly"]["temperature_2m"][i]);
        tmp.weathercode = weather["hourly"]["weathercode"][i];
        tmp.winddirection = weather["hourly"]["winddirection_10m"][i];
        tmp.windspeed = static_cast<int>((weather["hourly"]["windspeed_10m"][i]));
        vec.emplace_back(tmp);
    }
}

std::string GetTm(const std::string& date) {
    std::tm tm{};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M");
    std::time_t time = mktime(&tm);
    std::tm* res = std::localtime(&time);
    tm.tm_year += 1900;
    tm.tm_wday = res->tm_wday;
    return week[tm.tm_wday];
}

void PrintData(const std::vector<weather_block>& v) {
    for (size_t i = 0; i < 160; ++i) {
        if (i < 73 || i > 86) {
            std::cout << " ";
        } else {
            std::cout << "_";
        }
    }

    std::cout << std::endl;

    for (size_t i = 0; i < 160; ++i) {
        if (i < 73 || i > 86) {
            std::cout << " ";
        } else if (i == 73 || i == 84) {
            std::cout << "|";
        } else if (i == 75) {
            std::string today = GetTm(v[day * 24].time);
            std::cout << today.substr(0, 3) << '.';
            i += 3;
        } else if (i == 80) {
            std::cout << v[day * 24].time.substr(8, 2);
            i++;
        } else if (i == 83) {
            std::string month = v[day * 24].time.substr(5, 2);
            std::cout << months[month];
        } else {
            std::cout << " ";
        }
    }

    std::cout << std::endl;

    for (size_t i = 0; i < 160; ++i) {
        std::cout << "-";
    }

    std::cout << std::endl;

    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 160; ++j) {
            if (i == 0 && (j == 0 || j == 40 || j == 80 || j == 121 || j == 159)) {
                std::cout << "|";
            } else if (i == 0 && j == 18) {
                std::cout << "Night";
                j += 4;
            } else if (i == 0 && j == 57) {
                std::cout << "Morning";
                j += 6;
            } else if (i == 0 && j == 99) {
                std::cout << "Day";
                j += 2;
            } else if (i == 0 && j == 137) {
                std::cout << "Evening";
                j += 6;
            } else if (i == 0) {
                std::cout << " ";
            } else {
                std::cout << std::endl;
                for (size_t l = 0; l < 160; ++l) {
                    std::cout << "-";
                }

                break;
            }
        }
    }

    std::cout << std::endl;
}

size_t WeatherCode(const size_t code) {
    if (code == 0) {
        return 2;
    } else if (code == 1 || code == 2 || code == 3) {
        return 1;
    } else if (code == 45 || code == 48) {
        return 4;
    } else if (code == 51 || code == 53 || code == 55) {
        return 7;
    } else if (code == 56 || code == 57) {
        return 5;
    } else if (code == 61 || code == 63 || code == 65 || code == 66 || code == 67 || code == 80 || code == 81 || code == 82) {
        return 6;
    } else if (code == 71 || code == 73 || code == 75 || code == 77 || code == 85 || code == 86) {
        return 3;
    } else {
        return 8;
    }
}

void WeatherString(std::string& s, size_t code) {
    if (code == 1) {
        s += "Cloudy ";
    } else if (code == 2) {
        s += "Sunny ";
    } else if (code == 3) {
        s += "Snowfall ";
    } else if (code == 4) {
        s += "Hazy ";
    } else if (code == 5) {
        s += "Snowy ";
    } else if (code == 6) {
        s += "Heavy rain ";
    } else if (code == 7) {
        s += "Rainy ";
    } else {
        s += "Thunder ";
    }
}

void PrintWeather(const std::vector<weather_block>& v) {
    PrintData(v);
    std::vector<size_t> pic;
    for (size_t i = 0; i < 4; ++i) {
         pic.emplace_back(WeatherCode(v[day * kHours + i * 6].weathercode));
    }

    std::string s;
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            s = "| " + pictures[pic[j]][i];

            if (i == 0) {
                WeatherString(s, pic[j]);
            } else if (i == 1) {
                int max = INT_MIN;
                int min = INT_MAX;
                for (size_t l = day * 24 + j * 6; l < day * 24 + j * 6 + 6; ++l) {
                    max = std::max(max, v[l].temperature);
                    min = std::min(min, v[l].temperature);
                }

                s += std::to_string(max) + "(" + std::to_string(min) + ") C";

            } else if (i == 2) {
                size_t narrow = v[day * kHours + j * 6].winddirection;
                if ((narrow >= 0 && narrow <= 22) || (narrow >= 338 && narrow <= 360)) {
                    s += "N ";
                } else if (narrow >= 23 && narrow <= 67) {
                    s += "NE ";
                } else if (narrow >= 68 && narrow <= 112) {
                    s += "E ";
                } else if (narrow >= 113 && narrow <= 157){
                    s += "SE ";
                } else if (narrow >= 158 && narrow <= 202){
                    s += "S ";
                } else if (narrow >= 203 && narrow <= 247){
                    s += "SW ";
                } else if (narrow >= 248 && narrow <= 292){
                    s += "W ";
                } else {
                    s += "NW ";
                }

                int max = INT_MIN;
                int min = INT_MAX;
                for (size_t l = day * kHours + j * 6; l < day * kHours + j * 6 + 6; ++l) {
                    max = std::max(max, v[l].windspeed);
                    min = std::min(min, v[l].windspeed);
                }

                s += std::to_string(min) + "-" + std::to_string(max) + "km/h";

            } else if (i == 3) {
                s += std::to_string(v[day * kHours + j * 6].precipitation) + " mm" + std::to_string(v[day * kHours + j * 6].precipitation_probability) + "%";
            } else {
                s += std::to_string(v[day * kHours + j * 6].surface_pressure) + " mm" + std::to_string(v[day * kHours + j * 6].relativehumidity) + "%";
            }

            std::cout << s;
            if (j != 3) {
                for (size_t l = 0; l < 40 - s.length(); ++l) {
                    std::cout << " ";
                }
            } else {
                for (size_t l = 0; l < 39 - s.length(); ++l) {
                    std::cout << " ";
                }
            }

            if (j == 3) {
                std::cout << "|\n";
            }

        }
    }

    for (size_t i = 0; i < 160; ++i) {
        std::cout << "-";
    }

    std::cout << std::endl;
}

void GetInfo(const nlohmann::json& conf, const std::vector<std::string>& cities, std::vector<std::vector<weather_block>>& v, const int k) {
    for (size_t j = 0; j < cities.size(); ++j) {
        cpr::Response r = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                                   cpr::Parameters{{"name", conf["cities"][j]}},
                                   cpr::Header{{"X-Api-Key", "0WiAjNnL4UnqBQO3dpNcng==l4xYyR84VRdau7Xz"}});
        if (r.status_code != cpr::status::HTTP_OK) {
            std::string error = "No internet: please be sure to have good connection";
            std::cout << error;
            exit(0);
        }
        nlohmann::json responseJson = nlohmann::json::parse(r.text);

        std::string latitude = std::to_string(static_cast<double>(responseJson[0]["latitude"]));
        std::string longitude = std::to_string(static_cast<double>(responseJson[0]["longitude"]));

        std::string wheather_url =
            "https://api.open-meteo.com/v1/forecast?latitude=" + latitude + "&longitude=" + longitude
                + "&hourly=temperature_2m," + "relativehumidity_2m," + "precipitation_probability,"
                + "precipitation," + "weathercode," + "surface_pressure," + "windspeed_10m,"
                + "winddirection_10m"
                + "&forecast_days=10";
        cpr::Response w = cpr::Get(cpr::Url{wheather_url});
        nlohmann::json weather = nlohmann::json::parse(w.text);
        GetInformation(weather, v[j], conf, k);
    }
}

int main() {
    std::filesystem::path config_path = "/Users/stanislavvakulenko/CLionProjects/labwork-10/cmake-build-debug/Config.txt";
    std::ifstream nput(config_path, std::ios::binary);
    nlohmann::json conf = nlohmann::json::parse(nput);
    std::vector<std::string> cities(conf["cities"].size());
    for (size_t i = 0; i < cities.size(); ++i) {
        cities[i] = conf["cities"][i];
    }

    int c;
    int i = 0;
    int k = conf["days"];
    int frequency = conf["frequency"];
    const int delta = frequency;
    std::vector<std::vector<weather_block>> v(cities.size());
    GetInfo(conf, cities, v, kRequest);
    std::cout << '\n' << cities[i] << "\n\n";
    for (size_t l = 0; l < k; ++l) {
        PrintWeather(v[i]);
        day++;
    }
    day = 0;

    while (true) {
        c = getch();
        if (c != 10) {
            if (c == 43) {
                k = std::min(kRequest, k + 1);
            } else if (c == 45) {
                k = std::max(k - 1, 1);
            } else if (c == 27) {
                break;
            } else if (c == 110) {
                i = (i + 1) % conf["cities"].size();
            } else if (c == 112) {
                i = (i - 1) % conf["cities"].size();
            }

            system("clear");
            std::cout << '\n' << cities[i] << "\n\n";
            for (size_t l = 0; l < k; ++l) {
                PrintWeather(v[i]);
                day++;
            }

            day = 0;

            if (clock() / 1e6 - frequency >= delta) {
                frequency = clock() / 1e6;
                std::vector<std::vector<weather_block>> cur(cities.size());
                v = cur;
                GetInfo(conf, cities, v, kRequest);
            }
        }
    }
}
