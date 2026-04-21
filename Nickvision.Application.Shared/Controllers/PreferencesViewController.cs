using Microsoft.Data.Sqlite;
using Nickvision.Application.Shared.Helpers;
using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Globalization;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nickvision.Application.Shared.Controllers;

public class PreferencesViewController : IDisposable
{
    private readonly IConfigurationService _configurationService;
    private readonly ITranslationService _translationService;
    private SqliteTransaction? _transaction;

    public IReadOnlyList<SelectionItem<string>> AvailableTranslationLanguages { get; }
    public IReadOnlyList<SelectionItem<Theme>> Themes { get; }

    public PreferencesViewController(IConfigurationService configurationService, ITranslationService translationService)
    {
        _configurationService = configurationService;
        _translationService = translationService;
        _transaction = _configurationService.CreateTransaction();
        AvailableTranslationLanguages = new List<SelectionItem<string>>()
        {
            new SelectionItem<string>(string.Empty, _translationService._("System"), string.IsNullOrEmpty(_configurationService.TranslationLanguage)),
            new SelectionItem<string>("C", "en_US", _configurationService.TranslationLanguage == "C")
        };
        var languages = _translationService.AvailableLanguages.ToList();
        languages.Sort();
        foreach (var language in languages)
        {
            (AvailableTranslationLanguages as IList)!.Add(new SelectionItem<string>(language, language, _configurationService.TranslationLanguage == language));
        }
        Themes = new List<SelectionItem<Theme>>()
        {
            new SelectionItem<Theme>(Models.Theme.Light, _translationService._p("Theme", "Light"), _configurationService.Theme == Models.Theme.Light),
            new SelectionItem<Theme>(Models.Theme.Dark, _translationService._p("Theme", "Dark"), _configurationService.Theme == Models.Theme.Dark),
            new SelectionItem<Theme>(Models.Theme.System, _translationService._p("Theme", "System"), _configurationService.Theme == Models.Theme.System),
        };
    }

    ~PreferencesViewController()
    {
        Dispose(false);
    }

    public SelectionItem<Theme> Theme
    {
        set => _configurationService.Theme = value.Value;
    }

    public SelectionItem<string> TranslationLanguage
    {
        set => _configurationService.TranslationLanguage = value.Value;
    }

    public bool AllowPreviewUpdates
    {
        get => _configurationService.AllowPreviewUpdates;

        set => _configurationService.AllowPreviewUpdates = value;
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    public async Task SaveConfigurationAsync()
    {
        if(_transaction is not null)
        {
            await _transaction.CommitAsync();
            await _transaction.DisposeAsync();
            _transaction = null;
        }
    }
    private void Dispose(bool disposing)
    {
        if(!disposing)
        {
            return;
        }
        _transaction?.Dispose();
    }
}
