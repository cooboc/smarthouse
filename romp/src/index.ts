import { gearServer } from "./gear_pool";
import { webServer } from "./webui";
gearServer.begin();
webServer.begin();