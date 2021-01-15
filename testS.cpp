#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>


void wrightToFile(std::string lon[],std::string lat[], int* arrayPnew, int iteration, int elements)
{
    iteration++;
    std::ofstream fileN;
    fileN.open(std::to_string(iteration),std::ios::trunc);

    for (int it= 0 ; it < elements; it++)
    {   
        fileN << lat[it] <<" "<<lon[it]<<"  "<<arrayPnew[it]+1<<","<<std::endl;
    }

    
    fileN.close();

}
std::string ReadJSON(char* filename)
{        
    int jsonElements=0;
    std::stringstream getJson;
    std::string str;
    std::size_t found;
    std::string rawJson;
    std::ifstream reader(filename );
    if (reader.is_open())
    {
        while (std::getline(reader, str))
        {
            found =str.find("lat");
            getJson<<str<<std::endl;;
            if (found != std::string::npos)
            {
                ++jsonElements;
            }
        }
        rawJson = getJson.str();
        reader.close();
    }
    
    std::size_t foundlat = 1;
    std::size_t foundDot1 = 1;
    std::size_t foundComa1 = 1;
    std::string lat[jsonElements];

    std::size_t foundlon =1;
    std::size_t foundDot2 = 1;
    std::string lon[jsonElements];

    std::size_t foundDists = rawJson.find("distant")+17;


    int distMat[jsonElements][jsonElements];
    std::string rawDistdata="";
    int digitsnif;

    for(int i=1; i <= jsonElements; i++)
    {
        foundlat = rawJson.find("\"lat\":",foundDot1);
        foundDot1 =6 + foundlat;
        foundComa1 =rawJson.find(",",foundDot1);
        lat[i-1]= rawJson.substr(foundDot1,foundComa1-foundDot1);
        

        foundlon = rawJson.find("\"lon\":",foundDot2);
        foundDot2 =6 + foundlon;
        foundComa1 =rawJson.find('\n',foundDot2);
        lon[i-1]= rawJson.substr(foundDot2,foundComa1-foundDot2-1);



        foundDists = rawJson.find('[',foundDists)+1;
        //std::cout<< rawJson.substr(foundDists,30)<<std::endl;
        digitsnif=0;
        for(int j=1; j <= jsonElements; j++)
        {

            while (isdigit( rawJson[digitsnif+foundDists])== 0 ) 
            {
                digitsnif++;
            }

            while ((isdigit( rawJson[digitsnif+foundDists]) != 0 ) )
            {
                digitsnif++;
                rawDistdata += rawJson[digitsnif+foundDists-1] ;
            }

            //std::cout<<std::endl<<rawDistdata<<std::endl;
            distMat[i-1][j-1] = std::stoi(rawDistdata);
            //std::cout<<std::to_string( distMat[i-1][j-1] )<<std::endl;
            rawDistdata.clear();
        }

        //std::cout<<lon[i-1]<<std::endl;
        //std::cout<<toascii(found)<<std::endl;


        //distMat
        //lon lar
    }
    


//
//          HORROR
//

    
    int minArray[jsonElements];
    bool boolArray[jsonElements];
    int destIndex[jsonElements];

    int indexes=jsonElements;

    int depth = 0;
    int KIndex[jsonElements];
    int arrayPnew[jsonElements];
    int arrayPold[jsonElements];

    for(int i = 0; i < indexes; i++)
    {
        arrayPnew[i]=i;
    }
    int min;












    wrightToFile(lon,lat, arrayPnew, 0,jsonElements);

    while(depth< 29 &&indexes != 1)
    {
        depth++;
    //
    //          Calculation of mins
    //    



        for(int i = 0; i < indexes; i++)
        {
            min = std::numeric_limits<int>::max();
                
            for(int j = 0; j < indexes; j++)
            {

                if (min > distMat[i][j]&& i !=j )
                {
                    min= distMat[i][j];
                    minArray[i]= min;
                    destIndex[i] =j; 
                }
            }

        }

        


    //
    // Seting new values
    //
    for(int i = 0; i < indexes; i++)
        {
            boolArray[i]= true;
        }

        int newIndexes=0;
        int minimalIndex;
        for(int i = 0; i < indexes; i++)
        {   
            min = std::numeric_limits<int>::max();
            minimalIndex=-1;
            for(int j = 0; j < indexes; j++)
            {
                if (min > minArray[j]&& boolArray[j]==true &&boolArray[destIndex[j]] )
                {
                    minimalIndex = j;
                }
            }
            if (minimalIndex>-1)
            {
                min=minArray[minimalIndex];
                boolArray[minimalIndex]=false;
                boolArray[destIndex[minimalIndex]]=false;
                destIndex[destIndex[minimalIndex]]= minimalIndex;

            }
        }
        for(int i = 0; i < indexes; i++)
        {
                if (boolArray[i] == true)
                {

                    KIndex[i]=newIndexes;
                    newIndexes++;

                }
                else
                {
                    
                    if (destIndex[i] > i)
                    {
                    
                    KIndex[i]=newIndexes;

                    newIndexes++;
                    }
                    else{
                        KIndex[i] = KIndex[ destIndex[i]];
                    }
                    
                }
                std::cout<<KIndex[i]<<"Kind"<<std::endl;
//            std::cout<<minArray[i]<<"asdasdasdas"<<std::endl;
                
        }
        for(int iter= 0 ; iter < jsonElements; iter++)
        {

            arrayPnew[iter] =KIndex[arrayPnew[iter]]; 
            std::cout<<arrayPnew[iter]<<"aRnEw"<<std::endl;
        }


    //   new Matrix
        int defaultValueMatrix = std::numeric_limits<int>::max();
        int tempMat[newIndexes][newIndexes];

        for(int i = 0; i < newIndexes; i++)
        {
            for(int j = 0; j < newIndexes; j++)
            {
                tempMat[i][j] = defaultValueMatrix;

            }
        }
        for(int i = 0; i < indexes; i++)
        {
            for(int j = 0; j < indexes; j++)
            {
                if (tempMat[KIndex[i]][KIndex[j]] > distMat[i][j]&& KIndex[i] != KIndex[j] )
                {
                    tempMat[KIndex[i]][KIndex[j]]= distMat[i][j];
                }

            }
        }
        for(int i = 0; i < newIndexes; i++)
        {
            for(int j = 0; j < newIndexes; j++)
            {
                distMat[i][j] = tempMat[i][j] ;
                std::cout<<tempMat[i][j]<<"                              ";
            }
        }

        indexes = newIndexes;
        wrightToFile(lon,lat, arrayPnew, depth,jsonElements);
        std::cout<<std::endl;
    }


    return rawJson;
}





int main(int c, char **v)
{

    if (c != 2){
        std::cout << "Open with filename as parameter"<<std::endl;
            return 0;
    }

    std::string jasonText = ReadJSON( v[1]);

}

