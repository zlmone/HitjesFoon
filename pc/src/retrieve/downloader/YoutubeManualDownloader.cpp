#include "retrieve/downloader/YoutubeManualDownloader.h"

YoutubeManualDownloader::YoutubeManualDownloader(GUI &gui) : Downloader(gui) {

}

YoutubeManualDownloader::~YoutubeManualDownloader() {

}


//bool YoutubeAPI::searchVid(Hitje &hitje, const string &query, const string &hitjesPath) {
//    // Signal that we are downloading
//    hitje.downloadState.downloading = true;
//    gui.setHitje(hitje);
//    // we only need the total amount of results and each results id (kind and channelId/videoId)
//    string json = makeRequest(SEARCH, "id,snippet", "pageInfo/totalResults,items(id,snippet/title)", 5, query.c_str());
//    JsonParser result(gui);
//    result.parse(json.c_str());
//    if (result.getTotalResults() == 0) {
//        throw "Nothing found while searching";
//    }
//
//    string hitjeFilename = hitje.createFilename();
//    gui.printlevel(LBGINFO, "Done searching and found %s\n", hitjeFilename.c_str());
//    // create path for saving audio data
//    string hitjePath = hitjesPath + hitjeFilename;
//
//    string filename;
//    vector<string> videoIds = result.getVideoIds();
//    vector<string> titles = result.getVideoTitles();
//

//    string videoInfo = getVideoInfo(videoIds[0].c_str());
//    filename = getFileFromVideoInfo(videoInfo.c_str());
//    if (filename.empty()) {
//        return false;
//    }
//
//    // use ffmpeg to get the audio file from video
//    string decodeCmd = "ffmpeg -n -i ";
//    decodeCmd += '"' + filename + '"';
//    decodeCmd += " -f mp3 -vn ";
//    decodeCmd += '"' + hitjePath + '"';
//    gui.printlevel(LDEBUG, "calling ffmpeg with '%s'\n", decodeCmd.c_str());
//    system(decodeCmd.c_str());
//
//    FILE *musicFile = fopen(hitjePath.c_str(), "r");
//    bool fileExists = musicFile != NULL;
//    SAFE_CLOSE(musicFile);
//
//    if (!fileExists) {
//        throw "File conversion failed";
//    }
//
//    // video file is not needed anymore, remove it
//    remove(filename.c_str());
//    gui.printlevel(LBGINFO, "Basename: %s\n", filename.c_str());
//    gui.printlevel(LBGINFO, "Removed old file\n");
//
//    hitje.downloadState.downloading = false;
//    return true;
//}
//
//string YoutubeAPI::makeRequest(RequestType requestType, const char *part, const char *fields, unsigned char maxResults, const char *extra) {
//    string typeStr, extraArgs, baseUrl;
//
//    switch (requestType) {
//        case SEARCH: {
//                typeStr = "search";
//                string urlEscaped = transfer->escape(extra);
//                extraArgs = "q=" + urlEscaped + "&type=video";
//            }
//            break;
//        default:
//            throw "Unknown request type";
//    }
//
//    char fieldsStr[100];
//
//    if (fields == 0) {
//        // no fields parameter given, omit it from request
//        fieldsStr[0] = 0;
//    } else {
//        // add fields parameters to fields= field
//        sprintf(fieldsStr, "&fields=%s", fields);
//    }
//
//    baseUrl = "https://www.googleapis.com/youtube/v3/";
//    baseUrl += typeStr;
//    baseUrl += '?' + extraArgs;
//    baseUrl += "&part=";
//    baseUrl += part;
//    baseUrl += fieldsStr;
//    baseUrl += "&key=";
//    baseUrl += DEVELOPER_KEY;
//    baseUrl += "&maxResults=";
//    baseUrl += to_string(maxResults);
//
//    gui.printlevel(LDEBUG, "\n%s\n", baseUrl.c_str());
//    string jsonString = transfer->get(baseUrl.c_str());
//    gui.printlevel(LDEBUG, "%s", jsonString.c_str());
//    return jsonString;
//}
//
//string YoutubeAPI::getVideoInfo(const char *videoId) {
//    string baseUrl = "http://www.youtube.com/get_video_info?video_id=";
//    baseUrl += videoId;
//    baseUrl += "&asv=3&el=detailpage&hl=en_US";
//
//    string videoInfo = transfer->get(baseUrl.c_str());
//
//    FILE *file = fopen("videoInfo.txt", "wb");
//    fputs(videoInfo.c_str(), file);
//    SAFE_CLOSE(file);
//    return videoInfo;
//}
//
//string YoutubeAPI::getFileFromVideoInfo(const char *videoInfo) {
//    char *fmt = strstr(videoInfo, "url_encoded_fmt_stream_map=");
//    if (!fmt) {
//        throw "fmt not found";
//    }
//    char *fmtEnd = strchr(fmt, '&');
//    if (!fmtEnd) {
//        fmtEnd = strchr(fmt, '\0');
//    }
//
//    char *titleStart = strstr(videoInfo, "title=") + 6;
//    if (!titleStart) {
//        throw "No title start found";
//    }
//    char *titleEnd = strchr(titleStart, '&');
//    if (!titleEnd) {
//        titleEnd = strchr(titleStart, '\0');
//    } else {
//        *(titleEnd) = '\0';
//    }
//    char encoded[fmtEnd - fmt + 1];
//    strncpy(encoded, fmt, fmtEnd - fmt);
//    encoded[fmtEnd - fmt] = 0;
//
//    fmt = fmtEnd = NULL;
//
//    const char *decoded = urlDecode(encoded).c_str();
//    FILE *file = fopen("decoded.txt", "wb");
//    fputs(decoded, file);
//    SAFE_CLOSE(file);
//
//    char *urlStart = NULL, *urlEnd = NULL;
//    char *itagStart = NULL, *itagEnd = NULL;
//
//    //map<int, char*> urls;
//    const int priority[] = {38, 37, 46, 22, 45, 44, 35, 18, 34, 43, 6, 5, 36, 17, 13, 0};
//    int highestPriority = 1000;
//    char *highestUrl = NULL;
//    char *farEnd = strchr(decoded, ',');
//
//    char sig[300];
//    sig[0] = '\0';
//
//    //TODO: strlen eruit halen!!
//    int len = strlen(decoded);
//    for (char *itr = decoded; itr <= len + decoded; itr++) {
//        switch (*itr) {
//            case 'u':
//                // check for url
//                if (!strncmp(itr, "url=", 4)) {
//                    gui.printlevel(LDEBUG, "url found \n");
//                    if (urlStart || urlEnd) {
//                        throw "two urls found";
//                    }
//                    urlEnd = strchr(itr, '&');
//                    if ((farEnd && (farEnd < urlEnd)) || !urlEnd) {
//                        // if last character of this decoded youtube information is earlier than the end, end it there
//                        urlEnd = farEnd;
//                    }
//                    urlStart = itr + 4;
//                    itr = urlEnd - 1;
//                }
//                break;
//            case 's':
//                // check for signature
//                {
//
//                    bool decrypt = !strncmp(itr, "s=", 2);
//                    if (decrypt || !strncmp(itr, "sig=", 4)) {
//                        gui.printlevel(LDEBUG, "sig found \n");
//                        if (sig[0]) {
//                            throw "two sigs found";
//                        }
//                        char *sigEnd = strchr(itr, '&');
//                        if ((farEnd && (farEnd < sigEnd)) || !sigEnd) {
//                            // if last character of this decoded youtube information is earlier than the end, end it there
//                            sigEnd = farEnd;
//                        }
//                        if (decrypt) {
//                            // encrypted signature, not fully implemented yet
//                            // TODO: implement player url extraction
//                            decryptSignature(itr + 2, sigEnd - (itr + 2), sig);
//                            if (!sig[0]) {
//                                gui.printlevel(LDEBUG, "couldn't decrypt sig\n");
//                            }
//                        } else {
//                            strncpy(sig, itr + 4, sigEnd - (itr + 4));
//                            sig[sigEnd - (itr + 4)] = '\0';
//                        }
//                        itr = sigEnd - 1;
//                    }
//                    break;
//                }
//            case 'i':
//                // check for signature
//                if (!strncmp(itr, "itag=", 5)) {
//                    gui.printlevel(LDEBUG, "itag found\n");
//                    if (itagStart || itagEnd) {
//                        throw "two itag found";
//                    }
//                    itagEnd = strchr(itr, '&');
//                    if ((farEnd && (farEnd < itagEnd)) || !itagEnd) {
//                        // if last character of this decoded youtube information is earlier than the end, end it there
//                        itagEnd = farEnd;
//                    }
//                    itagStart = itr + 5;
//                    itr = itagEnd - 1;
//                }
//                break;
//            case '\0':
//            case ',':
//                gui.printlevel(LDEBUG, "Found comma\n");
//                // split on comma, save only when url and sig found
//                if (urlStart && urlEnd && sig[0] && itagStart && itagEnd) {
//                    // only parse if all information found
//                    int itag;
//                    sscanf(itagStart, "%d", &itag);
//
//                    int i;
//                    for (i = 0; priority[i] && priority[i] != itag; i++);
//                    if (priority[i] && i <= highestPriority) {
//
//                        *urlEnd = 0;
//                        char *decodedUrl = urlDecode(urlStart).c_str();
//
//                        SAFE_DELETE_ARRAY(highestUrl);
//
//                        int urlLen = strlen(decodedUrl);
//                        highestUrl = new char[urlLen + 7 + titleEnd - titleStart + 11 + strlen(sig) + 1];
//
//                        strncpy(highestUrl, decodedUrl, urlLen);
//                        strncpy(highestUrl + urlLen, "&title=", 7);
//                        strncpy(highestUrl + urlLen + 7, titleStart, titleEnd - titleStart);
//                        strncpy(highestUrl + urlLen + 7 + (titleEnd - titleStart), "&signature=", 11);
//                        strcpy(highestUrl + urlLen + 7 + (titleEnd - titleStart) + 11, sig);
//                        highestUrl[urlLen + 7 + (titleEnd - titleStart) + 11 + strlen(sig)] = 0;
//
//                        SAFE_DELETE_ARRAY(decodedUrl);
//
//                        highestPriority = i;
//
//                        gui.printlevel(LBGINFO, "\nNew url inserted, %d: %s\n", itag, highestUrl);
//                    } else {
//                        gui.printlevel(LBGINFO, "\nData with lower quality found\n");
//                    }
//                } else {
//                    gui.printlevel(LWARNING, "\nNot all information found\n");
//                }
//
//                urlStart = urlEnd = itagStart = itagEnd = NULL;
//                sig[0] = '\0';
//                if (itr != len + decoded) {
//                    farEnd = strchr(itr + 1, ',');
//                    if (!farEnd) {
//                        farEnd = strchr(itr + 1, '\0');
//                    }
//                }
//        }
//    }
//
//    string fileName;
//    if (highestUrl) {
//        gui.printlevel(LBGINFO, "Best url found: \n%s\n", highestUrl);
//        fileName = downloadEncodedUrl(highestUrl, titleStart);
//        SAFE_DELETE_ARRAY(highestUrl);
//    }
//
//    SAFE_DELETE_ARRAY(decoded);
//
//    return fileName;
//}
//
//string YoutubeAPI::downloadEncodedUrl(const char *url, const char *title) {
//    int titleLen = strlen(title);
//    char fileName[titleLen + 4 + 1];
//    strncpy(fileName, title, titleLen);
//    strncpy(fileName + titleLen, ".mp3", 4);
//    fileName[titleLen + 4] = 0;
//
//    string decodedFilename = urlDecode(fileName);
//    // replace + with spaces in title
//    replace_if(decodedFilename.begin(), decodedFilename.end(), [](const char &c) {
//        return (c == '+') || !((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '(' || c == ')' || c == '-' || c == '_' || c == '#' || c == '.');
//    }, ' ');
//
//    gui.printlevel(LBGINFO, "\nStarted downloading '%s'\n", decodedFilename.c_str());
//    transfer->get(url, decodedFilename.c_str());
//    gui.printlevel(LBGINFO, "Downloading done!\n");
//    return decodedFilename;
//}
//
//
//string YoutubeAPI::urlDecode(const char *src) {
//    unsigned int len = strlen(src);
//    char *ret = new char[len + 1];
//    unsigned int i, j;
//    char num[3];
//    num[2] = 0;
//    for (j = i = 0; i < len; i++, j++) {
//        if (src[i] == '%') {
//            num[0] = src[i + 1];
//            num[1] = src[i + 2];
//            unsigned int decoded;
//            sscanf(num, "%x", &decoded);
//            *(ret + j) = (char)decoded;
//            i = i + 2;
//        } else {
//            ret[j] = src[i];
//        }
//    }
//    ret[j] = 0;
//    return string(ret);
//}
//
//
//void YoutubeAPI::decryptSignature(char *sigstart, int siglen, char *decrypted) {
//    return;
//    gui.printlevel(LDEBUG, "decrypted sig length %d\n", siglen);
//    switch (siglen) {
//        case 93:
//            reverse_copy(sigstart + 30, sigstart + 87, decrypted);      // 56 chars
//            decrypted[56] = sigstart[88];                               // 57 chars
//            reverse_copy(sigstart + 6, sigstart + 29, decrypted + 57);  // 87 chars
//            decrypted[87] = '\0';
//            break;
//        case 92:
//            decrypted[0] = sigstart[25];                                // 1 char
//            copy(sigstart + 3, sigstart + 25, decrypted + 1);           // 23 chars
//            decrypted[23] = sigstart[0];                                // 24 chars
//            copy(sigstart + 26, sigstart + 42, decrypted + 24);         // 40 chars
//            decrypted[40] = sigstart[79];                               // 41 chars
//            copy(sigstart + 43, sigstart + 79, decrypted + 41);         // 77 chars
//            decrypted[77] = sigstart[91];                               // 78 chars
//            copy(sigstart + 80, sigstart + 83, decrypted + 78);         // 81 chars
//            decrypted[81] = '\0';
//            break;
//        case 91:
//            reverse_copy(sigstart + 28, sigstart + 85, decrypted);      // 57 chars
//            decrypted[57] = sigstart[86];                               // 58 chars
//            reverse_copy(sigstart + 6, sigstart + 27, decrypted + 58);  // 79 chars
//            decrypted[79] = '\0';
//            break;
//        case 85:
//            copy(sigstart + 3, sigstart + 11, decrypted);               // 8 chars
//            decrypted[8] = sigstart[0];                                 // 9 chars
//            copy(sigstart + 12, sigstart + 55, decrypted + 9);          // 52 chars
//            decrypted[52] = sigstart[84];                               // 53 chars
//            copy(sigstart + 56, sigstart + 84, decrypted + 53);         // 81 chars
//            decrypted[81] = '\0';
//            break;
//        case 84:
//            reverse_copy(sigstart + 71, sigstart + 79, decrypted);      // 8 chars
//            decrypted[8] = sigstart[14];                                // 9 chars
//            reverse_copy(sigstart + 38, sigstart + 70, decrypted + 9);  // 41 chars
//            decrypted[41] = sigstart[70];                               // 42 chars
//            reverse_copy(sigstart + 15, sigstart + 37, decrypted + 42); // 64 chars
//            decrypted[64] = sigstart[80];                               // 65 chars
//            reverse_copy(sigstart + 0, sigstart + 14, decrypted + 65);  // 79 chars
//            decrypted[79] = '\0';
//        case 83:
//            reverse_copy(sigstart + 64, sigstart + 81, decrypted);      // 17 chars
//            decrypted[17] = sigstart[0];                                // 18 chars
//            reverse_copy(sigstart + 1, sigstart + 63, decrypted + 18);  // 80 chars
//            decrypted[80] = sigstart[63];                               // 81 chars
//            decrypted[81] = '\0';
//            break;
//    }
//    gui.printlevel(LDEBUG, "after decryption: %s\n", decrypted);
    //exit(1);
//elif len(s) == 90:
//    return s[25] + s[3: 25] + s[2] + s[26: 40] + s[77] + s[41: 77] + s[89] + s[78: 81]
//       elif len(s) == 89:
//       return s[84: 78: -1] + s[87] + s[77: 60: -1] + s[0] + s[59: 3: -1]
//              elif len(s) == 88:
//                  return s[7: 28] + s[87] + s[29: 45] + s[55] + s[46: 55] + s[2] + s[56: 87] + s[28]
//                     elif len(s) == 87:
//                     return s[6: 27] + s[4] + s[28: 39] + s[27] + s[40: 59] + s[2] + s[60: ]
//                            elif len(s) == 86:
//                            return s[80: 72: -1] + s[16] + s[71: 39: -1] + s[72] + s[38: 16: -1] + s[82] + s[15:: -1]
//                                                   elif len(s) == 82:
//                                                   return s[80: 37: -1] + s[7] + s[36: 7: -1] + s[0] + s[6: 0: -1] + s[37]
//                                                           elif len(s) == 81:
//                                                           return s[56] + s[79: 56: -1] + s[41] + s[55: 41: -1] + s[80] + s[40: 34: -1] + s[0] + s[33: 29: -1] + s[34] + s[28: 9: -1] + s[29] + s[8: 0: -1] + s[9]
//                                                                   elif len(s) == 80:
//                                                                       return s[1: 19] + s[0] + s[20: 68] + s[19] + s[69: 80]
//                                                                           elif len(s) == 79:
//                                                                           return s[54] + s[77: 54: -1] + s[39] + s[53: 39: -1] + s[78] + s[38: 34: -1] + s[0] + s[33: 29: -1] + s[34] + s[28: 9: -1] + s[29] + s[8: 0: -1] + s[9]
//
//                                                                                   else:
//                                                                                           raise ExtractorError(u'Unable to decrypt signature, key length %d not supported; retrying might work' % (len(s)))
//}
