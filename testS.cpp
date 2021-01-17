#include <fstream>
#include <string>
#include <iostream>
#include <limits>
#include <vector>


void wrightToFile(std::vector<std::string> lon,std::vector<std::string> lat, std::vector<int> arrayToOu, int iteration, int elements)
{
    iteration++;
    std::ofstream fileN;
    fileN.open(std::to_string(iteration),std::ios::trunc);

    for (int it= 0 ; it < elements; it++)
    {   
        fileN << lat[it] <<" "<<lon[it]<<"  "<<arrayToOu[it] + 1 <<","<<std::endl;
    }
    fileN.close();

}
std::string ReadJSON(char* filename)
{     
    
    //--------------------------------   
    //Reading
    int jsonElements=0;
    std::string str;
    std::size_t found;
    std::string rawJson;
    std::ifstream reader(filename );
    if (reader.is_open())
    {
        while (std::getline(reader, str))
        {
            found =str.find("lat");
            rawJson+=str+'\n';
            if (found != std::string::npos)
            {
                ++jsonElements;
            }
        }
        reader.close();
    }
    else{throw "file not available";}
    //--------------------------------   
    //Parsing
    std::size_t foundlat;
    std::size_t foundDot1= 1;
    std::size_t foundComa1 ;
    std::vector<std::string> lat(jsonElements);

    std::size_t foundlon;
    std::size_t foundDot2 = 1;
    std::vector<std::string> lon(jsonElements);
    std::size_t foundDists = rawJson.find("distant")+17;

    std::vector<std::vector<int> > distMat(jsonElements, std::vector<int>(jsonElements));
    std::string rawDistdata;
    int digitsnif;

    
    for(int i=1; i <= jsonElements; i++)
    {
    //      Search for locatio data 

        foundlat = rawJson.find("\"lat\":",foundDot1);
        foundDot1 =6 + foundlat;
        foundComa1 =rawJson.find(",",foundDot1);
        lat.at(i-1)= rawJson.substr(foundDot1,foundComa1-foundDot1);
        

        foundlon = rawJson.find("\"lon\":",foundDot2);
        foundDot2 =6 + foundlon;
        foundComa1 =rawJson.find('\n',foundDot2);
        lon.at(i-1)= rawJson.substr(foundDot2,foundComa1-foundDot2-1);
        foundDists = rawJson.find('[',foundDists)+1;

        if (foundlon==std::string::npos||foundlat==std::string::npos){ throw "file was corupted";}

    //      Search for distance data  
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

            distMat[i-1][j-1] = std::stoi(rawDistdata);
            rawDistdata.clear();
        }
    }

    if(distMat.size() != jsonElements  ){
        throw "file was corupted";
    }
    




//
//          HORROR
//


    int indexes=jsonElements;
    
    std::vector<int> minArray (jsonElements);

    bool boolArray[jsonElements];
    std::vector<int> destIndex(jsonElements);


    int depth = 0;
    std::vector<int> KIndex(jsonElements);

    std::vector<int> arrayToOu(jsonElements);
    

    for(int i = 0; i < indexes; i++)
    {
        arrayToOu[i]=i;
    }
    int min;

    // Setting new values
    // 
    int newIndexes=0;
    int minimalIndex;

    //
    //   new Matrix

    int defaultValueMatrix = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> tempMat(jsonElements, std::vector<int>(jsonElements));



    if (jsonElements < 2){throw "Must be 2 or more elements";}
    wrightToFile(lon,lat, arrayToOu, 0,jsonElements);
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

                if (min > distMat.at(i).at(j)&& i != j )
                {
                    min= distMat.at(i).at(j);
                    minArray.at(i)= min;
                    destIndex.at(i) =j; 
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

        newIndexes=0;
        for(int i = 0; i < indexes; i++)
        {   
            min = std::numeric_limits<int>::max();
            minimalIndex=-1;
            for(int j = 0; j < indexes; j++)
            {
                if (min > minArray.at(j)&& boolArray[j]==true &&boolArray[destIndex.at(j)]==true  )
                {
                    minimalIndex = j;
                    min=minArray.at(minimalIndex);
                }
            }
            if (minimalIndex>-1)
            {

                boolArray[minimalIndex]=false;
                boolArray[destIndex.at(minimalIndex)]=false;
                destIndex.at(destIndex.at(minimalIndex))= minimalIndex;

            }
        }
        for(int i = 0; i < indexes; i++)
        {
                if (boolArray[i] == true)
                {
                    KIndex.at(i)=newIndexes;
                    newIndexes++;
                }
                else
                {
                    
                    if (destIndex.at(i) > i)
                    {
                    KIndex.at(i)=newIndexes;
                    newIndexes++;
                    }
                    else{
                        KIndex.at(i) = KIndex.at( destIndex.at(i));
                    }
                    
                }
                std::cout<<KIndex.at(i)<<"Kind"<<std::endl;
//            std::cout<<minArray.at(i]<<"asdasdasdas"<<std::endl;
                
        }
        for(int iter= 0 ; iter < jsonElements; iter++)
        {

            arrayToOu[iter] =KIndex.at(arrayToOu[iter]); 
            std::cout<<arrayToOu[iter]<<"aRnEw"<<std::endl;
        }

    //
    //   new Matrix
    //

        tempMat.resize(newIndexes, std::vector<int>(newIndexes));
        for(int i = 0; i < newIndexes; i++)
        {
            for(int j = 0; j < newIndexes; j++)
            {
                distMat.at(i).at(j) = defaultValueMatrix;
            }
        }

        for(int i = 0; i < indexes; i++)
        {
            for(int j = 0; j < indexes; j++)
            {
                if (tempMat.at(KIndex.at(i)).at(KIndex.at(j)) > distMat.at(i).at(j)&& KIndex.at(i) != KIndex.at(j) )
                {
                    tempMat.at(KIndex.at(i)).at(KIndex.at(j))= distMat.at(i).at(j);
                }
            }
        }
        distMat.resize(newIndexes, std::vector<int>(newIndexes));
        for(int i = 0; i < newIndexes; i++)
        {
                distMat.at(i) = tempMat.at(i) ;
        }
        indexes = newIndexes;
        wrightToFile(lon,lat, arrayToOu, depth,jsonElements);
        std::cout<<std::endl;
    }
    return rawJson;
}


int main(int c, char **v)
{
    if (c != 2)
    {
        std::cout << "Open with filename as parameter"<<std::endl;
            return 0;
    }
    try {std::string jasonText = ReadJSON( v[1]);}
    catch(const char* err) {
        std::cerr <<err<<std::endl;
        return 0;}
    return 1;
}

