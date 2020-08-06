//import Foundation
//
////FileManager.default.contents(atPath: <#T##String#>)
//
//func getAsset(filename:String) -> FileData {
//    let file = FileManager.default.contents(atPath:filename);
//    var result:FileData = FileData();
////    result.data = file as? UnsafeRawPointer;
//    result.data = file.bytes;
//    
//    // get file size
//    do {
//        let attr = try FileManager.default.attributesOfItem(atPath:filename);
//        result.size = attr[FileAttributeKey.size] as! Int;
//    } catch {
//        print("error: \(error)");
//    }
//    
//    return result;
//}
//
//func freeAsset(asset:FileData) {
//    
//}
