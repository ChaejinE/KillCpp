#include <iostream>
#include <string>

int main()
{
    std::u16string u16_str = u"안녕하세요 채진이 집들이 오신 것을 환영합니다";
    std::string jaum[] = {"ㄱ", "ㄲ", "ㄴ", "ㄷ", "ㄸ", "ㄹ", "ㅁ",
                        "ㅂ", "ㅃ", "ㅅ", "ㅆ", "ㅇ", "ㅈ", "ㅉ",
                        "ㅊ", "ㅋ", "ㅌ", "ㅍ", "ㅎ"};
    
    for (char16_t c : u16_str)
    {
        if (!(0xAC00 <= c && c <= 0xD7A3))
            continue; // 유니코드 상에서 한글의 범위
        
        int offset = c - 0XAC00;
        int jaum_offset = offset / 0x24C;

        std::cout << jaum[jaum_offset];
    }
}