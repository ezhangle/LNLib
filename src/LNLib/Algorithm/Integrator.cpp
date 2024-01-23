/*
 * Author:
 * 2024/01/08 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 * 微信公众号：BIMCoder梁老师
 *
 * Use of this source code is governed by a GPL-3.0 license that can be found in
 * the LICENSE file.
 */

#include "Integrator.h"
#include "FFT.h"

namespace LNLib
{
	double Integrator::Simpson(double start, double end, double startTangentLength, double middleTangentLength, double endTangentLength)
	{
		double result = ((end - start) / 6.0) * (startTangentLength + 4 * middleTangentLength + endTangentLength);
		return result;
	}

	double Integrator::Simpson(double start, double end, std::vector<double> odds, std::vector<double> evens, double delta)
	{
		double oddsSum = 0.0;
		double evensSum = 0.0;
		for (int i = 0; i < odds.size(); i++)
		{
			oddsSum += 4 * odds[i];
		}
		for (int i = 0; i < evens.size(); i++)
		{
			evensSum += 2 * evens[i];
		}
		double result = (delta / 3.0) * (start + oddsSum + evensSum + end);
		return result;
	}

    const std::vector<double> Integrator::GaussLegendreAbscissae =
    {
        -0.0640568928626056260850430826247450385909,
        0.0640568928626056260850430826247450385909,
        -0.1911188674736163091586398207570696318404,
        0.1911188674736163091586398207570696318404,
        -0.3150426796961633743867932913198102407864,
        0.3150426796961633743867932913198102407864,
        -0.4337935076260451384870842319133497124524,
        0.4337935076260451384870842319133497124524,
        -0.5454214713888395356583756172183723700107,
        0.5454214713888395356583756172183723700107,
        -0.6480936519369755692524957869107476266696,
        0.6480936519369755692524957869107476266696,
        -0.7401241915785543642438281030999784255232,
        0.7401241915785543642438281030999784255232,
        -0.8200019859739029219539498726697452080761,
        0.8200019859739029219539498726697452080761,
        -0.8864155270044010342131543419821967550873,
        0.8864155270044010342131543419821967550873,
        -0.9382745520027327585236490017087214496548,
        0.9382745520027327585236490017087214496548,
        -0.9747285559713094981983919930081690617411,
        0.9747285559713094981983919930081690617411,
        -0.9951872199970213601799974097007368118745,
        0.9951872199970213601799974097007368118745,
    };

    const std::vector<double> Integrator::GaussLegendreWeights =
    {
        0.1279381953467521569740561652246953718517,
        0.1279381953467521569740561652246953718517,
        0.1258374563468282961213753825111836887264,
        0.1258374563468282961213753825111836887264,
        0.121670472927803391204463153476262425607,
        0.121670472927803391204463153476262425607,
        0.1155056680537256013533444839067835598622,
        0.1155056680537256013533444839067835598622,
        0.1074442701159656347825773424466062227946,
        0.1074442701159656347825773424466062227946,
        0.0976186521041138882698806644642471544279,
        0.0976186521041138882698806644642471544279,
        0.086190161531953275917185202983742667185,
        0.086190161531953275917185202983742667185,
        0.0733464814110803057340336152531165181193,
        0.0733464814110803057340336152531165181193,
        0.0592985849154367807463677585001085845412,
        0.0592985849154367807463677585001085845412,
        0.0442774388174198061686027482113382288593,
        0.0442774388174198061686027482113382288593,
        0.0285313886289336631813078159518782864491,
        0.0285313886289336631813078159518782864491,
        0.0123412297999871995468056670700372915759,
        0.0123412297999871995468056670700372915759,
    };

    std::vector<double> Integrator::ChebyshevSeries(int size)
    {
        std::vector<double> series(size);

        int lenw = series.size() - 1;
        int j, k, l, m;
        double cos2, sin1, sin2, hl;

        cos2 = 0;
        sin1 = 1;
        sin2 = 1;
        hl = 0.5;
        k = lenw;
        l = 2;
        while (l < k - l - 1) 
        {
            series[0] = hl * 0.5;
            for (j = 1; j <= l; j++) 
            {
                series[j] = hl / (1 - 4 * j * j);
            }
            series[l] *= 0.5;
            dfct(l, 0.5 * cos2, sin1, series);
            cos2 = sqrt(2 + cos2);
            sin1 /= cos2;
            sin2 /= 2 + cos2;
            series[k] = sin2;
            series[k - 1] = series[0];
            series[k - 2] = series[l];
            k -= 3;
            m = l;
            while (m > 1) 
            {
                m >>= 1;
                for (j = m; j <= l - m; j += (m << 1)) 
                {
                    series[k] = series[j];
                    k--;
                }
            }
            hl *= 0.5;
            l *= 2;
        }
        return series;
    }
    
}


