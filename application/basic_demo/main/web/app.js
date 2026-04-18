/* ═══════════════════════════════════════════════════
   i18n – extensible locale registry
   To add a new language, append an entry to LOCALES
   and add a matching key in `strings`.
   ═══════════════════════════════════════════════════ */

const LOCALES = [
  { id: "en", label: "English" },
  { id: "zh-cn", label: "简体中文" },
];

const strings = {
  en: {
    docTitle: "ESP-Claw Settings",
    pageTitle: "Device Settings",
    pageSubtitle:
      "Configure credentials, runtime settings, and manage FATFS storage on your ESP device.",
    tabConfig: "Configuration",
    tabMemory: "Memory",
    tabFiles: "File Manager",

    statusLoading: "Loading\u2026",
    statusOnline: "Wi-Fi Connected",
    statusOffline: "Wi-Fi Offline",

    sectionWifi: "Wi-Fi Settings",
    wifiSsid: "Wi-Fi SSID",
    wifiPassword: "Wi-Fi Password",

    sectionLlm: "LLM Settings",
    llmProvider: "LLM Provider",
    llmProviderOpenai: "OpenAI",
    llmProviderQwen: "Qwen Compatible",
    llmProviderAnthropic: "Anthropic",
    llmProviderCustom: "Custom",
    llmApiKey: "API Key",
    llmModel: "Model",
    llmTimeout: "Timeout (ms)",
    llmTimeoutPlaceholder: "120000",
    llmAdvanced: "LLM Advanced Options",
    llmBackend: "Backend Type",
    llmBackendPlaceholder: "openai_compatible / anthropic / custom",
    llmProfile: "Profile",
    llmProfilePlaceholder: "openai / qwen_compatible / anthropic",
    llmBaseUrl: "Base URL",
    llmBaseUrlPlaceholder: "https://api.openai.com",
    llmAuthType: "Auth Type",
    llmAuthTypePlaceholder: "bearer / api-key / none",

    sectionIm: "Instant Messaging (IM)",
    imWechatTitle: "WeChat",
    imFeishuTitle: "Feishu",
    wechatLoginDesc: "QR Code Sign-in",
    qqAppId: "QQ App ID",
    qqAppSecret: "QQ App Secret",
    feishuAppId: "Feishu App ID",
    feishuAppSecret: "Feishu App Secret",
    tgBotToken: "Telegram Bot Token",
    wechatToken: "WeChat Token",
    wechatBaseUrl: "WeChat Base URL",
    wechatBaseUrlPlaceholder: "https://ilinkai.weixin.qq.com",
    wechatCdnBaseUrl: "WeChat CDN Base URL",
    wechatCdnBaseUrlPlaceholder: "https://novac2c.cdn.weixin.qq.com/c2c",
    wechatAccountId: "WeChat Account ID",
    wechatAccountIdPlaceholder: "default",
    wechatLogin: "WeChat Login",
    wechatLoginGenerate: "Generate QR",
    wechatLoginCancel: "Cancel",
    wechatLoginStatus: "Status: idle",
    wechatLoginNote:
      "Generate a QR code here to obtain wechat_token automatically. The token is saved to NVS after confirmation. Restart the device to apply the new login.",
    wechatLoginOpenLink: "Open login link",

    sectionSearch: "Search (Optional)",
    searchNote: "If set, ESP-Claw can search online.",
    searchBraveKey: "Brave Search API Key",
    searchTavilyKey: "Tavily API Key",

    sectionAdvanced: "Advanced Settings",
    timezone: "Timezone",
    timezonePlaceholder: "e.g. UTC0",
    luaBaseDir: "Lua Base Directory",

    saveBtn: "Save Changes",
    saveSuccess: "Settings saved",
    saveError: "Failed to save settings",
    memoryTitle: "Memory Files",
    memoryDescription: "Manage the device memory files under /memory.",
    memoryCardLongTerm: "Long-term Memory / 长期记忆",
    memoryMissing: "Long-term memory file does not exist yet.",
    memoryCreate: "Create",
    memorySave: "Save Memory",
    memoryRefresh: "Refresh",
    memoryAuxMissing: "This file does not exist yet.",
    memoryCreateEmpty: "Create",
    memoryCreateConfirm: "Memory does not exist. Create /memory/memory.md now?",
    memoryCreated: "Memory created",
    memoryEmptyCreated: "Empty file created",
    memorySaved: "Memory saved",
    memoryRefreshed: "Memory reloaded",
    memoryLoadError: "Failed to load memory",
    memorySaveError: "Failed to save memory",
    configNote:
      "Changes are stored in NVS. Choose an LLM provider first, then fill the shared API key and model fields. Restart the device after updating Wi-Fi, Feishu, or core LLM settings.",

    fileRefresh: "Refresh",
    fileUpDir: "Up One Level",
    fileNewFolder: "New folder name",
    fileCreateFolder: "Create Folder",
    fileUploadPath: "/example.txt",
    fileChoose: "Choose File",
    fileNoFileSelected: "No file selected",
    fileUpload: "Upload",
    fileColName: "Name",
    fileColType: "Type",
    fileColSize: "Size",
    fileColActions: "Actions",
    fileTypeFolder: "Folder",
    fileTypeFile: "File",
    fileEmpty: "This folder is empty.",
    fileOpen: "Open",
    fileEdit: "Edit",
    fileDownload: "Download",
    fileDelete: "Delete",
    fileDeleteConfirm: "Delete {path}?",
    fileUploadComplete: "Upload completed",
    fileFolderCreated: "Folder created",
    fileDeleteComplete: "Delete completed",
    fileEditorTitle: "Edit File",
    fileEditorRefresh: "Refresh",
    fileEditorClose: "Close",
    fileEditorSave: "Save",
    fileEditorSaved: "File saved",
    fileEditorLoadError: "Failed to load file",
    fileEditorSaveError: "Failed to save file",
    fileSelectAndPath:
      "Select a file and provide a target path that starts with /.",
    fileFolderNameRequired: "Enter a folder name.",
  },

  "zh-cn": {
    docTitle: "ESP-Claw 设置",
    pageTitle: "设备设置",
    pageSubtitle:
      "配置凭据、运行时设置，并管理 ESP 设备上的 FATFS 存储。",
    tabConfig: "配置管理",
    tabMemory: "记忆管理",
    tabFiles: "文件管理",

    statusLoading: "加载中\u2026",
    statusOnline: "Wi-Fi 已连接",
    statusOffline: "Wi-Fi 离线",

    sectionWifi: "Wi-Fi 设置",
    wifiSsid: "Wi-Fi 名称 (SSID)",
    wifiPassword: "Wi-Fi 密码",

    sectionLlm: "LLM 设置",
    llmProvider: "LLM 提供商",
    llmProviderOpenai: "OpenAI",
    llmProviderQwen: "Qwen 兼容",
    llmProviderAnthropic: "Anthropic",
    llmProviderCustom: "自定义",
    llmApiKey: "API 密钥",
    llmModel: "模型",
    llmTimeout: "超时 (毫秒)",
    llmTimeoutPlaceholder: "120000",
    llmAdvanced: "LLM 高级选项",
    llmBackend: "后端类型",
    llmBackendPlaceholder: "openai_compatible / anthropic / custom",
    llmProfile: "配置文件",
    llmProfilePlaceholder: "openai / qwen_compatible / anthropic",
    llmBaseUrl: "Base URL",
    llmBaseUrlPlaceholder: "https://api.openai.com",
    llmAuthType: "认证类型",
    llmAuthTypePlaceholder: "bearer / api-key / none",

    sectionIm: "即时通讯 (IM)",
    imWechatTitle: "微信",
    imFeishuTitle: "飞书",
    wechatLoginDesc: "二维码登录",
    qqAppId: "QQ App ID",
    qqAppSecret: "QQ App Secret",
    feishuAppId: "飞书 App ID",
    feishuAppSecret: "飞书 App Secret",
    tgBotToken: "Telegram Bot Token",
    wechatToken: "微信 Token",
    wechatBaseUrl: "微信 Base URL",
    wechatBaseUrlPlaceholder: "https://ilinkai.weixin.qq.com",
    wechatCdnBaseUrl: "微信 CDN Base URL",
    wechatCdnBaseUrlPlaceholder: "https://novac2c.cdn.weixin.qq.com/c2c",
    wechatAccountId: "微信账号 ID",
    wechatAccountIdPlaceholder: "默认",
    wechatLogin: "微信登录",
    wechatLoginGenerate: "生成二维码",
    wechatLoginCancel: "取消",
    wechatLoginStatus: "状态：空闲",
    wechatLoginNote:
      "在此生成二维码以自动获取 wechat_token。确认后令牌将保存至 NVS。重启设备以应用新的登录。",
    wechatLoginOpenLink: "打开登录链接",

    sectionSearch: "搜索（可选）",
    searchNote: "如填写，ESP-Claw 可在运行中检索在线资源。",
    searchBraveKey: "Brave Search API Key",
    searchTavilyKey: "Tavily API Key",

    sectionAdvanced: "高级设置",
    timezone: "时区",
    timezonePlaceholder: "例如 UTC0",
    luaBaseDir: "Lua 基本目录",

    saveBtn: "保存更改",
    saveSuccess: "设置已保存",
    saveError: "保存设置失败",
    memoryTitle: "记忆文件",
    memoryDescription: "管理 /memory 目录下的记忆文件。",
    memoryCardLongTerm: "长期记忆 / Long-term Memory",
    memoryMissing: "长期记忆文件尚不存在。",
    memoryCreate: "创建",
    memorySave: "保存记忆",
    memoryRefresh: "刷新",
    memoryAuxMissing: "该文件尚不存在。",
    memoryCreateEmpty: "创建",
    memoryCreateConfirm: "记忆不存在。现在创建 /memory/memory.md 吗？",
    memoryCreated: "记忆已创建",
    memoryEmptyCreated: "空文件已创建",
    memorySaved: "记忆已保存",
    memoryRefreshed: "记忆已重新加载",
    memoryLoadError: "加载记忆失败",
    memorySaveError: "保存记忆失败",
    configNote:
      "更改存储在 NVS 中。请先选择 LLM 提供商，然后填写 API 密钥和模型字段。更新 Wi-Fi、飞书或核心 LLM 设置后请重启设备。",

    fileRefresh: "刷新",
    fileUpDir: "返回上级",
    fileNewFolder: "新文件夹名称",
    fileCreateFolder: "创建文件夹",
    fileUploadPath: "/example.txt",
    fileChoose: "选择文件",
    fileNoFileSelected: "未选择文件",
    fileUpload: "上传",
    fileColName: "名称",
    fileColType: "类型",
    fileColSize: "大小",
    fileColActions: "操作",
    fileTypeFolder: "文件夹",
    fileTypeFile: "文件",
    fileEmpty: "此文件夹为空。",
    fileOpen: "打开",
    fileEdit: "编辑",
    fileDownload: "下载",
    fileDelete: "删除",
    fileDeleteConfirm: "确定删除 {path}？",
    fileUploadComplete: "上传完成",
    fileFolderCreated: "文件夹已创建",
    fileDeleteComplete: "删除完成",
    fileEditorTitle: "编辑文件",
    fileEditorRefresh: "刷新",
    fileEditorClose: "退出",
    fileEditorSave: "保存",
    fileEditorSaved: "文件已保存",
    fileEditorLoadError: "加载文件失败",
    fileEditorSaveError: "保存文件失败",
    fileSelectAndPath: "请选择文件并提供以 / 开头的目标路径。",
    fileFolderNameRequired: "请输入文件夹名称。",
  },
};

/* ── Current language & device state ── */
let currentLang = "en";
let wifiConnected = null; // null = unknown/loading, true/false = fetched

function t(key) {
  const table = strings[currentLang] || strings["en"];
  return table[key] ?? (strings["en"][key] || key);
}

function detectLang() {
  const params = new URLSearchParams(window.location.search);
  const fromUrl = params.get("lang");
  if (fromUrl && strings[fromUrl]) return fromUrl;

  const saved = localStorage.getItem("esp-claw-lang");
  if (saved && strings[saved]) return saved;

  const nav = (navigator.language || "").toLowerCase();
  if (nav.startsWith("zh")) return "zh-cn";
  return "en";
}

function applyI18n() {
  document.querySelectorAll("[data-i18n]").forEach((el) => {
    el.textContent = t(el.dataset.i18n);
  });
  document.querySelectorAll("[data-i18n-placeholder]").forEach((el) => {
    el.placeholder = t(el.dataset.i18nPlaceholder);
  });
  document.querySelectorAll("[data-i18n-title]").forEach((el) => {
    document.title = t(el.dataset.i18nTitle);
  });
  document.querySelectorAll("[data-i18n-option]").forEach((el) => {
    el.textContent = t(el.dataset.i18nOption);
  });
  document.documentElement.lang = currentLang === "zh-cn" ? "zh-CN" : "en";
  document.title = t("docTitle");
  refreshStatusText();
}

function refreshStatusText() {
  const el = document.getElementById("status-text");
  if (wifiConnected === null) {
    el.textContent = t("statusLoading");
  } else {
    el.textContent = wifiConnected ? t("statusOnline") : t("statusOffline");
  }
}

function buildLocaleMenu() {
  const menu = document.getElementById("locale-menu");
  menu.innerHTML = "";
  LOCALES.forEach((loc) => {
    const li = document.createElement("li");
    const a = document.createElement("a");
    a.textContent = loc.label;
    a.href = "#";
    a.setAttribute("data-lang", loc.id);
    if (loc.id === currentLang) a.classList.add("is-current");
    a.addEventListener("click", (e) => {
      e.preventDefault();
      setLang(loc.id);
      document.getElementById("lang-switcher").removeAttribute("open");
    });
    li.appendChild(a);
    menu.appendChild(li);
  });
  const label = LOCALES.find((l) => l.id === currentLang);
  document.getElementById("current-locale-label").textContent =
    label ? label.label : currentLang;
}

function setLang(lang) {
  if (!strings[lang]) return;
  currentLang = lang;
  localStorage.setItem("esp-claw-lang", lang);
  applyI18n();
  buildLocaleMenu();
  renderFileRows(lastFileEntries);
  renderMemoryState();
}

/* ═══════════════════════════════════════════════════
   Tab switching
   ═══════════════════════════════════════════════════ */

function initTabs() {
  document.querySelectorAll(".tab-btn").forEach((btn) => {
    btn.addEventListener("click", () => {
      document
        .querySelectorAll(".tab-btn")
        .forEach((b) => b.classList.remove("active"));
      document
        .querySelectorAll(".tab-panel")
        .forEach((p) => p.classList.remove("active"));
      btn.classList.add("active");
      const panel = document.getElementById("tab-" + btn.dataset.tab);
      if (panel) panel.classList.add("active");
      if (btn.dataset.tab === "memory") {
        loadMemory().catch((err) =>
          showBanner("memoryBanner", err.message || t("memoryLoadError"), true)
        );
      }
    });
  });
}

/* ═══════════════════════════════════════════════════
   Config – NVS fields
   ═══════════════════════════════════════════════════ */

const configFields = [
  "wifi_ssid",
  "wifi_password",
  "llm_api_key",
  "llm_backend_type",
  "llm_profile",
  "llm_model",
  "llm_base_url",
  "llm_auth_type",
  "llm_timeout_ms",
  "qq_app_id",
  "qq_app_secret",
  "feishu_app_id",
  "feishu_app_secret",
  "tg_bot_token",
  "wechat_token",
  "wechat_base_url",
  "wechat_cdn_base_url",
  "wechat_account_id",
  "search_brave_key",
  "search_tavily_key",
  "lua_base_dir",
  "time_timezone",
];

const llmProviderPresets = {
  openai: {
    llm_backend_type: "openai_compatible",
    llm_profile: "openai",
    llm_base_url: "https://api.openai.com",
    llm_auth_type: "bearer",
  },
  qwen: {
    llm_backend_type: "openai_compatible",
    llm_profile: "qwen_compatible",
    llm_base_url: "https://dashscope.aliyuncs.com",
    llm_auth_type: "bearer",
  },
  anthropic: {
    llm_backend_type: "anthropic",
    llm_profile: "anthropic",
    llm_base_url: "https://api.anthropic.com",
    llm_auth_type: "none",
  },
};

/* ── Banner helpers ── */

function showBanner(id, message, isError = false) {
  const banner = document.getElementById(id);
  banner.textContent = message;
  banner.classList.remove("success", "error");
  banner.classList.add(isError ? "error" : "success");
  banner.classList.add("visible");
}

function hideBanner(id) {
  const banner = document.getElementById(id);
  banner.classList.remove("visible", "success", "error");
}

/* ── Config form read / fill ── */

function readConfigForm() {
  const payload = {};
  configFields.forEach((field) => {
    const input = document.getElementById(field);
    payload[field] = input ? input.value.trim() : "";
  });
  return payload;
}

function fillConfigForm(data) {
  configFields.forEach((field) => {
    const input = document.getElementById(field);
    if (input && typeof data[field] === "string") {
      input.value = data[field];
    }
  });
  syncProviderPreset();
}

/* ── LLM provider preset logic ── */

function detectProviderPreset() {
  const backend = document.getElementById("llm_backend_type")?.value.trim();
  const profile = document.getElementById("llm_profile")?.value.trim();
  const baseUrl = document.getElementById("llm_base_url")?.value.trim();
  const authType = document.getElementById("llm_auth_type")?.value.trim();

  const match = Object.entries(llmProviderPresets).find(
    ([, preset]) =>
      preset.llm_backend_type === backend &&
      preset.llm_profile === profile &&
      preset.llm_base_url === baseUrl &&
      preset.llm_auth_type === authType
  );
  return match ? match[0] : "custom";
}

function syncProviderPreset() {
  const select = document.getElementById("llm_provider_preset");
  if (select) select.value = detectProviderPreset();
}

function applyProviderPreset(presetKey) {
  const preset = llmProviderPresets[presetKey];
  if (!preset) {
    syncProviderPreset();
    return;
  }
  Object.entries(preset).forEach(([field, value]) => {
    const input = document.getElementById(field);
    if (input) input.value = value;
  });
  syncProviderPreset();
}

/* ═══════════════════════════════════════════════════
   API calls
   ═══════════════════════════════════════════════════ */

async function loadStatus() {
  try {
    const response = await fetch("/api/status", { cache: "no-store" });
    const data = await response.json();
    const badge = document.getElementById("status-badge");
    const bar = document.getElementById("status-bar");

    wifiConnected = !!data.wifi_connected;
    if (wifiConnected) {
      badge.classList.remove("offline");
      badge.classList.add("online");
      bar.classList.add("is-online");
    } else {
      badge.classList.remove("online");
      badge.classList.add("offline");
      bar.classList.remove("is-online");
    }
    refreshStatusText();

    if (data.ip) {
      document.getElementById("info-ip").textContent = "IP: " + data.ip;
      document.getElementById("info-ip").style.display = "";
      document.getElementById("meta-sep-ip").style.display = "";
    }
    if (data.storage_base_path) {
      document.getElementById("info-storage").textContent =
        "Storage: " + data.storage_base_path;
      document.getElementById("info-storage").style.display = "";
      document.getElementById("meta-sep-storage").style.display = "";
    }
  } catch (err) {
    showBanner("configBanner", err.message || t("statusLoading"), true);
  }
}

async function loadConfig() {
  hideBanner("configBanner");
  const response = await fetch("/api/config", { cache: "no-store" });
  if (!response.ok) throw new Error("Failed to load settings");
  fillConfigForm(await response.json());
}

const WECHAT_FIELDS = [
  "wechat_token", "wechat_base_url", "wechat_cdn_base_url", "wechat_account_id",
];

async function reloadWechatFields() {
  const response = await fetch("/api/config", { cache: "no-store" });
  if (!response.ok) return;
  const data = await response.json();
  WECHAT_FIELDS.forEach((field) => {
    const input = document.getElementById(field);
    if (input && typeof data[field] === "string") {
      input.value = data[field];
    }
  });
}

async function saveConfig() {
  const button = document.getElementById("saveConfigButton");
  button.disabled = true;
  hideBanner("configBanner");

  try {
    const response = await fetch("/api/config", {
      method: "POST",
      cache: "no-store",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(readConfigForm()),
    });
    const responseText = await response.text();
    let result = {};
    if (responseText) {
      try {
        result = JSON.parse(responseText);
      } catch (parseError) {
        if (response.ok) throw parseError;
        result = { error: responseText };
      }
    }
    if (!response.ok)
      throw new Error(result.error || t("saveError"));
    showBanner("configBanner", result.message || t("saveSuccess"));
    syncProviderPreset();
  } catch (err) {
    showBanner("configBanner", err.message, true);
  } finally {
    button.disabled = false;
  }
}

/* ═══════════════════════════════════════════════════
   WeChat Login
   ═══════════════════════════════════════════════════ */

let wechatLoginPollTimer = null;

function stopWechatLoginPolling() {
  if (wechatLoginPollTimer) {
    clearTimeout(wechatLoginPollTimer);
    wechatLoginPollTimer = null;
  }
}

function renderWechatLoginStatus(data) {
  const qrImage = document.getElementById("wechatLoginQr");
  const qrLink = document.getElementById("wechatLoginQrLink");
  const meta = document.getElementById("wechatLoginMeta");

  if (data.qr_data_url) {
    qrImage.src =
      "https://api.qrserver.com/v1/create-qr-code/?size=320x320&data=" +
      encodeURIComponent(data.qr_data_url);
    qrImage.classList.remove("hidden");
    qrLink.href = data.qr_data_url;
    qrLink.textContent = t("wechatLoginOpenLink");
    qrLink.classList.remove("hidden");
  } else {
    qrImage.removeAttribute("src");
    qrImage.classList.add("hidden");
    qrLink.removeAttribute("href");
    qrLink.textContent = "";
    qrLink.classList.add("hidden");
  }

  meta.textContent = data.status
    ? (currentLang === "zh-cn" ? "状态：" : "Status: ") + data.status
    : t("wechatLoginStatus");

  if (data.message) {
    showBanner("wechatLoginBanner", data.message);
  } else {
    hideBanner("wechatLoginBanner");
  }

  if (data.completed && data.persisted) {
    reloadWechatFields().catch(() => {});
  }
}

async function pollWechatLoginStatus() {
  try {
    const response = await fetch("/api/wechat/login/status", {
      cache: "no-store",
    });
    const data = await response.json();
    if (!response.ok)
      throw new Error(data.error || "Failed to fetch WeChat login status");
    renderWechatLoginStatus(data);
    if (data.active || (data.completed && !data.persisted)) {
      wechatLoginPollTimer = setTimeout(pollWechatLoginStatus, 1500);
    } else {
      stopWechatLoginPolling();
    }
  } catch (err) {
    showBanner("wechatLoginBanner", err.message, true);
    stopWechatLoginPolling();
  }
}

async function startWechatLogin() {
  const button = document.getElementById("wechatLoginStartButton");
  button.disabled = true;
  hideBanner("wechatLoginBanner");

  try {
    const response = await fetch("/api/wechat/login/start", {
      method: "POST",
      cache: "no-store",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        account_id: document.getElementById("wechat_account_id").value.trim(),
        force: true,
      }),
    });
    const data = await response.json();
    if (!response.ok)
      throw new Error(data.error || "Failed to start WeChat login");
    renderWechatLoginStatus(data);
    stopWechatLoginPolling();
    wechatLoginPollTimer = setTimeout(pollWechatLoginStatus, 1000);
  } catch (err) {
    showBanner("wechatLoginBanner", err.message, true);
  } finally {
    button.disabled = false;
  }
}

async function cancelWechatLogin() {
  try {
    const response = await fetch("/api/wechat/login/cancel", {
      method: "POST",
      cache: "no-store",
    });
    const data = await response.json();
    if (!response.ok)
      throw new Error(data.error || "Failed to cancel WeChat login");
    renderWechatLoginStatus({
      status: currentLang === "zh-cn" ? "已取消" : "cancelled",
      message:
        data.message ||
        (currentLang === "zh-cn" ? "已取消微信登录。" : "WeChat login cancelled."),
      qr_data_url: "",
      completed: false,
      persisted: false,
    });
    stopWechatLoginPolling();
  } catch (err) {
    showBanner("wechatLoginBanner", err.message, true);
  }
}

/* ═══════════════════════════════════════════════════
   File Manager
   ═══════════════════════════════════════════════════ */

let currentPath = "/";
let lastFileEntries = [];
const EDITABLE_FILE_EXTENSIONS = [".md", ".json", ".txt", ".jsonb", ".jsonc", ".lua"];
const MEMORY_FILES = [
  {
    id: "memoryEditor",
    missingId: null,
    path: "/memory/memory.md",
    template: "# Long-term Memory\n## Summary Labels\n\n## Active Memories\n",
  },
  { id: "soulEditor", missingId: "soulMissingState", path: "/memory/soul.md", template: "" },
  { id: "identityEditor", missingId: "identityMissingState", path: "/memory/identity.md", template: "" },
  { id: "userEditor", missingId: "userMissingState", path: "/memory/user.md", template: "" },
];
let memoryLoaded = false;
let memoryContents = {};
let memoryFilePresence = {};
let activeEditorPath = "";

function humanSize(value) {
  if (value < 1024) return value + " B";
  if (value < 1024 * 1024) return (value / 1024).toFixed(1) + " KB";
  return (value / (1024 * 1024)).toFixed(1) + " MB";
}

function parentPath(path) {
  if (path === "/") return "/";
  const parts = path.split("/").filter(Boolean);
  parts.pop();
  return parts.length ? "/" + parts.join("/") : "/";
}

function joinPath(base, name) {
  return base === "/" ? "/" + name : base + "/" + name;
}

function isEditableFile(path) {
  const lowerPath = (path || "").toLowerCase();
  return EDITABLE_FILE_EXTENSIONS.some((ext) => lowerPath.endsWith(ext));
}

function createActionSlot(node = null, placeholderText = "") {
  const slot = document.createElement("span");
  slot.className = "action-slot";
  if (node) {
    slot.appendChild(node);
  } else {
    slot.classList.add("placeholder");
    slot.textContent = placeholderText;
  }
  return slot;
}

function renderFileRows(entries) {
  lastFileEntries = entries;
  const tbody = document.getElementById("fileTableBody");
  tbody.innerHTML = "";

  if (!entries.length) {
    const row = document.createElement("tr");
    const td = document.createElement("td");
    td.setAttribute("colspan", "4");
    td.textContent = t("fileEmpty");
    td.style.color = "var(--text-muted)";
    row.appendChild(td);
    tbody.appendChild(row);
    return;
  }

  entries
    .sort(
      (a, b) =>
        Number(b.is_dir) - Number(a.is_dir) || a.name.localeCompare(b.name)
    )
    .forEach((entry) => {
      const row = document.createElement("tr");

      const tdName = document.createElement("td");
      tdName.textContent = entry.name;
      row.appendChild(tdName);

      const tdType = document.createElement("td");
      tdType.textContent = entry.is_dir ? t("fileTypeFolder") : t("fileTypeFile");
      row.appendChild(tdType);

      const tdSize = document.createElement("td");
      tdSize.textContent = entry.is_dir ? "-" : humanSize(entry.size || 0);
      row.appendChild(tdSize);

      const tdActions = document.createElement("td");
      tdActions.className = "actions";
      let editControl = null;
      let middleControl = null;

      if (entry.is_dir) {
        const openBtn = document.createElement("button");
        openBtn.className = "link-btn";
        openBtn.textContent = t("fileOpen");
        openBtn.onclick = () => {
          currentPath = entry.path;
          loadFiles().catch((err) =>
            showBanner("fileBanner", err.message, true)
          );
        };
        middleControl = openBtn;
      } else {
        if (isEditableFile(entry.path)) {
          const editBtn = document.createElement("button");
          editBtn.className = "link-btn";
          editBtn.textContent = t("fileEdit");
          editBtn.onclick = () => {
            openFileEditor(entry.path).catch((err) =>
              showBanner("fileBanner", err.message || t("fileEditorLoadError"), true)
            );
          };
          editControl = editBtn;
        }

        const dlLink = document.createElement("a");
        dlLink.href = "/files" + entry.path;
        dlLink.textContent = t("fileDownload");
        dlLink.className = "link-btn";
        dlLink.target = "_blank";
        middleControl = dlLink;
      }

      const delBtn = document.createElement("button");
      delBtn.className = "link-btn danger";
      delBtn.textContent = t("fileDelete");
      delBtn.onclick = async () => {
        const msg = t("fileDeleteConfirm").replace("{path}", entry.path);
        if (!window.confirm(msg)) return;
        await deletePath(entry.path);
      };
      const editPlaceholder = entry.is_dir ? t("fileTypeFolder") : t("fileEdit");
      tdActions.appendChild(createActionSlot(editControl, editPlaceholder));
      tdActions.appendChild(createActionSlot(middleControl));
      tdActions.appendChild(createActionSlot(delBtn));

      row.appendChild(tdActions);
      tbody.appendChild(row);
    });
}

async function loadFiles() {
  hideBanner("fileBanner");
  document.getElementById("currentPath").textContent = currentPath;

  const response = await fetch(
    "/api/files?path=" + encodeURIComponent(currentPath),
    { cache: "no-store" }
  );
  if (!response.ok) throw new Error((await response.text()) || "Failed to load file list");

  const data = await response.json();
  currentPath = data.path || "/";
  document.getElementById("currentPath").textContent = currentPath;
  renderFileRows(data.entries || []);
}

async function uploadFile() {
  const pathInput = document.getElementById("uploadPathInput");
  const fileInput = document.getElementById("uploadFileInput");
  const button = document.getElementById("uploadButton");
  const file = fileInput.files[0];
  const relativePath =
    pathInput.value.trim() || (file ? joinPath(currentPath, file.name) : "");

  if (!file || !relativePath.startsWith("/")) {
    showBanner("fileBanner", t("fileSelectAndPath"), true);
    return;
  }

  button.disabled = true;
  hideBanner("fileBanner");
  try {
    const response = await fetch(
      "/api/files/upload?path=" + encodeURIComponent(relativePath),
      { method: "POST", cache: "no-store", body: file }
    );
    if (!response.ok) throw new Error(await response.text());
    pathInput.value = "";
    fileInput.value = "";
    document.getElementById("selectedFileName").textContent =
      t("fileNoFileSelected");
    showBanner("fileBanner", t("fileUploadComplete"));
    await loadFiles();
  } catch (err) {
    showBanner("fileBanner", err.message || "Upload failed", true);
  } finally {
    button.disabled = false;
  }
}

async function createFolder() {
  const input = document.getElementById("newFolderInput");
  const name = input.value.trim();
  if (!name) {
    showBanner("fileBanner", t("fileFolderNameRequired"), true);
    return;
  }

  try {
    const response = await fetch("/api/files/mkdir", {
      method: "POST",
      cache: "no-store",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ path: joinPath(currentPath, name) }),
    });
    if (!response.ok) throw new Error(await response.text());
    input.value = "";
    showBanner("fileBanner", t("fileFolderCreated"));
    await loadFiles();
  } catch (err) {
    showBanner("fileBanner", err.message || "Failed to create folder", true);
  }
}

async function deletePath(path) {
  try {
    const response = await fetch(
      "/api/files?path=" + encodeURIComponent(path),
      { method: "DELETE", cache: "no-store" }
    );
    if (!response.ok) throw new Error(await response.text());
    showBanner("fileBanner", t("fileDeleteComplete"));
    await loadFiles();
  } catch (err) {
    showBanner("fileBanner", err.message || "Delete failed", true);
  }
}

/* ═══════════════════════════════════════════════════
   File Editor
   ═══════════════════════════════════════════════════ */

function openFileEditorModal() {
  document.getElementById("fileEditorModal").classList.remove("hidden");
  document.body.style.overflow = "hidden";
}

function closeFileEditorModal() {
  document.getElementById("fileEditorModal").classList.add("hidden");
  document.body.style.overflow = "";
  activeEditorPath = "";
  hideBanner("fileEditorBanner");
}

async function loadFileIntoEditor(path) {
  const response = await fetch("/files" + path, { cache: "no-store" });
  if (!response.ok) {
    throw new Error((await response.text()) || t("fileEditorLoadError"));
  }

  const content = await response.text();
  document.getElementById("fileEditorPath").textContent = path;
  document.getElementById("fileEditorTextarea").value = content;
  activeEditorPath = path;
}

async function openFileEditor(path) {
  hideBanner("fileEditorBanner");
  await loadFileIntoEditor(path);
  openFileEditorModal();
}

async function refreshFileEditor() {
  const button = document.getElementById("refreshFileEditorButton");
  if (!activeEditorPath) return;

  button.disabled = true;
  hideBanner("fileEditorBanner");
  try {
    await loadFileIntoEditor(activeEditorPath);
  } catch (err) {
    showBanner("fileEditorBanner", err.message || t("fileEditorLoadError"), true);
  } finally {
    button.disabled = false;
  }
}

async function saveFileEditor() {
  const button = document.getElementById("saveFileEditorButton");
  const textarea = document.getElementById("fileEditorTextarea");
  if (!activeEditorPath) return;

  button.disabled = true;
  hideBanner("fileEditorBanner");
  try {
    const response = await fetch(
      "/api/files/upload?path=" + encodeURIComponent(activeEditorPath),
      {
        method: "POST",
        cache: "no-store",
        body: new Blob([textarea.value], { type: "text/plain; charset=utf-8" }),
      }
    );
    if (!response.ok) {
      throw new Error((await response.text()) || t("fileEditorSaveError"));
    }
    showBanner("fileEditorBanner", t("fileEditorSaved"));
    await loadFiles();
  } catch (err) {
    showBanner("fileEditorBanner", err.message || t("fileEditorSaveError"), true);
  } finally {
    button.disabled = false;
  }
}

/* ═══════════════════════════════════════════════════
   Memory Manager
   ═══════════════════════════════════════════════════ */

function renderMemoryState() {
  MEMORY_FILES.forEach((file) => {
    const editor = document.getElementById(file.id);
    const missingId =
      file.missingId || (file.id === "memoryEditor" ? "memoryMissingState" : null);
    const missing = missingId ? document.getElementById(missingId) : null;
    const exists = !!memoryFilePresence[file.path];

    if (editor) {
      editor.value = memoryContents[file.path] || "";
      editor.disabled = !exists;
      editor.classList.toggle("hidden", !exists);
    }
    if (missing) {
      missing.classList.toggle("hidden", exists);
    }
  });
}

async function ensureMemoryDir() {
  const response = await fetch("/api/files/mkdir", {
    method: "POST",
    cache: "no-store",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ path: "/memory" }),
  });
  if (!response.ok) {
    throw new Error((await response.text()) || t("memorySaveError"));
  }
}

async function loadMemory() {
  hideBanner("memoryBanner");

  const dirResponse = await fetch("/api/files?path=" + encodeURIComponent("/memory"), {
    cache: "no-store",
  });

  if (!dirResponse.ok) {
    if (dirResponse.status === 404) {
      memoryContents = {};
      memoryFilePresence = {};
      MEMORY_FILES.forEach((file) => {
        memoryContents[file.path] = "";
        memoryFilePresence[file.path] = false;
      });
      memoryLoaded = true;
      renderMemoryState();
      return;
    }
    throw new Error((await dirResponse.text()) || t("memoryLoadError"));
  }

  const dirData = await dirResponse.json();
  const entries = dirData.entries || [];
  const entryNames = new Set(entries.map((entry) => entry.name));
  memoryContents = {};
  memoryFilePresence = {};

  for (const file of MEMORY_FILES) {
    const fileName = file.path.split("/").pop();
    const exists = entryNames.has(fileName);
    memoryFilePresence[file.path] = exists;
    if (!exists) {
      memoryContents[file.path] = "";
      continue;
    }

    const response = await fetch("/files" + file.path, {
      cache: "no-store",
    });
    if (!response.ok) {
      throw new Error((await response.text()) || t("memoryLoadError"));
    }
    memoryContents[file.path] = await response.text();
  }

  memoryLoaded = true;
  renderMemoryState();
}

async function uploadMemoryContent(path, content) {
  const response = await fetch(
    "/api/files/upload?path=" + encodeURIComponent(path),
    {
      method: "POST",
      cache: "no-store",
      body: new Blob([content], { type: "text/markdown; charset=utf-8" }),
    }
  );
  if (!response.ok) {
    throw new Error((await response.text()) || t("memorySaveError"));
  }
}

async function createMemory() {
  if (!window.confirm(t("memoryCreateConfirm"))) {
    return;
  }

  const button = document.getElementById("createMemoryButton");
  button.disabled = true;
  hideBanner("memoryBanner");

  try {
    await ensureMemoryDir();
    const primaryFile = MEMORY_FILES[0];
    await uploadMemoryContent(primaryFile.path, primaryFile.template);
    memoryContents[primaryFile.path] = primaryFile.template;
    memoryFilePresence[primaryFile.path] = true;
    memoryLoaded = true;
    renderMemoryState();
    showBanner("memoryBanner", t("memoryCreated"));
  } catch (err) {
    showBanner("memoryBanner", err.message || t("memorySaveError"), true);
  } finally {
    button.disabled = false;
  }
}

async function saveMemory() {
  const button = document.getElementById("saveMemoryButton");
  button.disabled = true;
  hideBanner("memoryBanner");

  try {
    await ensureMemoryDir();
    for (const file of MEMORY_FILES) {
      if (!memoryFilePresence[file.path]) {
        continue;
      }
      const editor = document.getElementById(file.id);
      const content = editor ? editor.value : "";
      await uploadMemoryContent(file.path, content);
      memoryContents[file.path] = content;
    }
    memoryLoaded = true;
    renderMemoryState();
    showBanner("memoryBanner", t("memorySaved"));
  } catch (err) {
    showBanner("memoryBanner", err.message || t("memorySaveError"), true);
  } finally {
    button.disabled = false;
  }
}

async function createEmptyMemoryFile(editorId) {
  const file = MEMORY_FILES.find((item) => item.id === editorId);
  if (!file || !file.missingId) {
    return;
  }

  const button = document.querySelector(`.memory-init-btn[data-editor="${editorId}"]`);
  if (button) {
    button.disabled = true;
  }
  hideBanner("memoryBanner");

  try {
    await ensureMemoryDir();
    await uploadMemoryContent(file.path, "");
    memoryFilePresence[file.path] = true;
    memoryContents[file.path] = "";
    renderMemoryState();
    showBanner("memoryBanner", t("memoryEmptyCreated"));
  } catch (err) {
    showBanner("memoryBanner", err.message || t("memorySaveError"), true);
  } finally {
    if (button) {
      button.disabled = false;
    }
  }
}

async function refreshMemory() {
  const button = document.getElementById("refreshMemoryButton");
  button.disabled = true;
  hideBanner("memoryBanner");

  try {
    await loadMemory();
    showBanner("memoryBanner", t("memoryRefreshed"));
  } catch (err) {
    showBanner("memoryBanner", err.message || t("memoryLoadError"), true);
  } finally {
    button.disabled = false;
  }
}

/* ═══════════════════════════════════════════════════
   Event binding
   ═══════════════════════════════════════════════════ */

function bindEvents() {
  initTabs();

  document.getElementById("saveConfigButton").addEventListener("click", saveConfig);

  document.getElementById("llm_provider_preset").addEventListener("change", (e) => {
    applyProviderPreset(e.target.value);
  });

  ["llm_backend_type", "llm_profile", "llm_base_url", "llm_auth_type"].forEach(
    (field) => {
      const input = document.getElementById(field);
      if (input) input.addEventListener("input", syncProviderPreset);
    }
  );

  document
    .getElementById("wechatLoginStartButton")
    .addEventListener("click", startWechatLogin);
  document
    .getElementById("wechatLoginCancelButton")
    .addEventListener("click", cancelWechatLogin);

  document.getElementById("refreshFilesButton").addEventListener("click", () =>
    loadFiles().catch((err) => showBanner("fileBanner", err.message, true))
  );
  document.getElementById("upDirButton").addEventListener("click", () => {
    currentPath = parentPath(currentPath);
    loadFiles().catch((err) => showBanner("fileBanner", err.message, true));
  });
  document.getElementById("uploadButton").addEventListener("click", uploadFile);
  document.getElementById("createMemoryButton").addEventListener("click", createMemory);
  document.getElementById("saveMemoryButton").addEventListener("click", saveMemory);
  document.getElementById("refreshMemoryButton").addEventListener("click", refreshMemory);
  document.querySelectorAll(".memory-init-btn").forEach((button) => {
    button.addEventListener("click", () => createEmptyMemoryFile(button.dataset.editor));
  });
  document
    .getElementById("closeFileEditorButton")
    .addEventListener("click", closeFileEditorModal);
  document
    .getElementById("refreshFileEditorButton")
    .addEventListener("click", refreshFileEditor);
  document
    .getElementById("saveFileEditorButton")
    .addEventListener("click", saveFileEditor);
  document.getElementById("fileEditorModal").addEventListener("click", (e) => {
    if (e.target.id === "fileEditorModal") {
      closeFileEditorModal();
    }
  });
  document.getElementById("chooseFileButton").addEventListener("click", () => {
    document.getElementById("uploadFileInput").click();
  });
  document.getElementById("createFolderButton").addEventListener("click", createFolder);
  document.getElementById("uploadFileInput").addEventListener("change", (e) => {
    const file = e.target.files[0];
    const nameSpan = document.getElementById("selectedFileName");
    if (file) {
      document.getElementById("uploadPathInput").value = joinPath(
        currentPath,
        file.name
      );
      nameSpan.textContent = file.name;
    } else {
      nameSpan.textContent = t("fileNoFileSelected");
    }
  });
}

/* ═══════════════════════════════════════════════════
   Bootstrap
   ═══════════════════════════════════════════════════ */

async function bootstrap() {
  currentLang = detectLang();
  applyI18n();
  buildLocaleMenu();
  bindEvents();

  try {
    await loadStatus();
  } catch (err) {
    showBanner("configBanner", err.message || "Failed to load device status", true);
  }

  try {
    await loadConfig();
  } catch (err) {
    showBanner("configBanner", err.message || "Failed to load settings", true);
  }

  try {
    await pollWechatLoginStatus();
  } catch (err) {
    showBanner(
      "wechatLoginBanner",
      err.message || "Failed to load WeChat login status",
      true
    );
  }

  try {
    await loadFiles();
  } catch (err) {
    showBanner("fileBanner", err.message || "Failed to load file list", true);
  }

  if (document.querySelector('.tab-btn[data-tab="memory"]')?.classList.contains("active")) {
    try {
      await loadMemory();
    } catch (err) {
      showBanner("memoryBanner", err.message || t("memoryLoadError"), true);
    }
  } else {
    renderMemoryState();
  }
}

bootstrap().catch((err) => {
  showBanner("configBanner", err.message || "Failed to initialize the page", true);
});
